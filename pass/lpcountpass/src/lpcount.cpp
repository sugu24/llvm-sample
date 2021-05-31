#include "lpcount.hpp"

bool LPCountPass::runOnLoop(llvm::Loop *L, llvm::LPPassManager &LPM){
	// 終了ブロックを取得
	llvm::BasicBlock *exiting_block = L->getExitingBlock();
	if (exiting_block == NULL)
		return false;

	// 終了ブロックのTerminatorInstを取得
	llvm::BranchInst *branch_inst = llvm::dyn_cast<llvm::BranchInst>(exiting_block->getTerminator());
	if(branch_inst == NULL)
		return false;

	// conditionalか確認
	// operand数が3であるか確認しているだけ
	if(!branch_inst->isConditional())
		return false;
	
	// cmp命令の取得
	llvm::ICmpInst *icmp = llvm::dyn_cast<llvm::ICmpInst>(branch_inst->getOperand(0));
	if(icmp == NULL)
		return false;
	
	// -------- ExitingBlockとbr命令の取得 -------- //
	// 第2オペランドは終了値を表す定数のはず
	// もう一方はPHINodeを想定
	llvm::Value *lhs = icmp->getOperand(0);
	llvm::Value *rhs = icmp->getOperand(1);
	llvm::PHINode *phin = llvm::dyn_cast<llvm::PHINode>(lhs);
	llvm::ConstantInt *exit_val = llvm::dyn_cast<llvm::ConstantInt>(rhs);
	if(!(phin && exit_val))
		return false;

	// PredicateはSLTであるはず
	llvm::CmpInst::Predicate pred = icmp->getPredicate();
	if(pred != llvm::ICmpInst::ICMP_SLT)
		return false;

	// PHINodeのどちらかが定数であってほしい
	lhs = phin->getIncomingValue(0);
	rhs = phin->getIncomingValue(1);

	// phinodeのオペランドの片方が初期値
	// もう一方がadd命令になるはず
	llvm::ConstantInt *init_val = llvm::dyn_cast<llvm::ConstantInt>(lhs);
	llvm::BinaryOperator *bin_op = llvm::dyn_cast<llvm::BinaryOperator>(rhs);
	if(!(init_val && bin_op)){
		init_val = llvm::dyn_cast<llvm::ConstantInt>(rhs);
		bin_op = llvm::dyn_cast<llvm::BinaryOperator>(lhs);
		return false;
	}
	
	// いずれも一致せず
	if(!init_val && bin_op)
		return false;

	// add命令であることを確認
	if(bin_op == NULL || bin_op->getOpcode() != llvm::Instruction::Add)
		return false;

	// %val = add %i constの形であると仮定する
	// 制御変数への代入がなければadd命令の第1オペランドは先ほど取得したPHINodeのはず
	if(bin_op->getOperand(0) != phin)
		return false;

	// 第2オペランドの定数が繰り返しごとの増加値になる
	llvm::ConstantInt *step = llvm::dyn_cast<llvm::ConstantInt>(bin_op->getOperand(1));
	if(step == NULL)
		return false;

	// 初期値と終了値と増加値が得られたので繰り返し回数を得られるはず
	if(step->getZExtValue() == 0)
		return false;

	uint64_t result = (exit_val->getZExtValue() - init_val->getZExtValue()) /
	       	(step->getZExtValue());
	if((exit_val->getZExtValue() - init_val->getZExtValue()) % (step->getZExtValue()))
		result++;

	llvm::errs() << "loop count = " << result << "\n";

	return false;
}

void LPCountPass::getAnalysisUsage(llvm::AnalysisUsage &AU) const{
	// 変更を加えない
	AU.setPreservesAll();
}

char LPCountPass::ID = 0;
static llvm::RegisterPass<LPCountPass> X("lpcount", "lpcount pass : count loop trip", false, false);
