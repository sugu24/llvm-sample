#include "easypass.hpp"

bool EasyPass::runOnFunction(llvm::Function &F){
	llvm::errs() << "Function Name " << F.getName().str() << "\n";
	return false;
}

void EasyPass::getAnalysisUsage(llvm::AnalysisUsage &AU) const{
	// 変更を加えないためsetPreserveAll
	AU.setPreservesAll();
}

char EasyPass::ID = 0;
static llvm::RegisterPass<EasyPass> X("easypass", "easy pass : only print function name", false, false);
