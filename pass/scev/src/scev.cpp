#include "scev.hpp"

bool SCEV::runOnLoop(llvm::Loop *L, llvm::LPPassManager &LPM){
	// getAnalysisでScalarEvolutionを取得
	llvm::ScalarEvolution *SE = &(getAnalysis<llvm::ScalarEvolution>());

	// ExitingBlockを取得
	llvm::BasicBlock *exiting_bb = L->getExitingBlock();
	if(exiting_bb == NULL)
		return false;

	unsigned int trip_count = SE->getSmallConstantTripCount(L,exiting_bb);

	llvm::errs() << "loop trip count = " << trip_count << "\n";


	return false;
}

void SCEV::getAnalysisUsage(llvm::AnalysisUsage &AU) const{
	// 変更を加えない
	AU.setPreservesAll();

	// ScalarEvolutionを利用するのでaddRequired
	AU.addRequired<llvm::ScalarEvolution>();
}

char SCEV::ID = 0;
static llvm::RegisterPass<SCEV> X("scev", "analyze loop trip count using scalar evolution", false, false);
