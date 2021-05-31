#include "DCElimination.hpp"

bool DCEliminationPass :: runOnFunction(llvm::Function &F){
	// 対象の命令を格納するlist
	std::list<llvm::Instruction*> works;

	// 命令を削除したらtrueに変更
	bool change = false;

	// 最初に全ての命令を調べる（Instructionをたどる）
	llvm::inst_iterator inst_itr = llvm::inst_begin(F);
	llvm::inst_iterator inst_itr_end = llvm::inst_end(F);
	for(; inst_itr != inst_itr_end; inst_itr++)
		works.push_back(&*inst_itr);
	
	/* これでもInstructionをたどれる
	llvm::Function::iterator bb_itr = F.begin();
	llvm::Function::iterator bb_itr_end = F.end();
	for(; bb_itr != bb_itr_end; bb_it++){
		llvm::BasicBlock::iterator inst_itr = bb_itr->begin();
		llvm::BasicBlock::iterator inst_itr = bb_itr->end();
		for(; inst_itr != inst_itr_end; inst_itr++)
			works.push_back(&*inst_itr);
	*/
	
	// worksに追加した命令からdead codeになったものがないか調べる
	llvm::Instruction *inst = NULL;
	while(!works.empty()){
		// 命令をpop
		inst = works.front();
		works.pop_front();
		
		// この命令の値を使用している命令がある場合は生きているとする
		// load / store命令、call命令、終端命令（return やbr）は生きている命令にする
		if( !inst->use_empty() || llvm::isa<llvm::TerminatorInst>(inst) || 
				llvm::isa<llvm::CallInst>(inst) ||
				llvm::isa<llvm::LoadInst>(inst) ||
				llvm::isa<llvm::StoreInst>(inst) )
			continue;
		
		// 削除する命令のオペランドがdead codeになる可能性があるので
		// オペランドをworksに追加する
		llvm::User::op_iterator op_itr = inst->op_begin();
		llvm::User::op_iterator op_itr_end = inst->op_end();
		for(; op_itr != op_itr_end; op_itr++){
			// すでにworksに登録済みなら何もしない
			llvm::Instruction *op_inst = llvm::dyn_cast<llvm::Instruction>(*op_itr);
			if(op_inst)
				if(std::find(works.begin(), works.end(), &*op_inst) == works.end())
					works.push_back(op_inst);
		}

		// 削除する
		change = true;
		inst->eraseFromParent();

	}
	// 命令を削除したらtrueにしておく
	return change;
}

/*
 * getAnalysisUsage関数
 */
void DCEliminationPass::getAnalysisUsage(llvm::AnalysisUsage &AU) const{
	// CFGに変更はない
	AU.setPreservesCFG();
}

char DCEliminationPass::ID = 0;
static llvm::RegisterPass<DCEliminationPass> X("dcelimination", "eliminate dead code", false, false);
