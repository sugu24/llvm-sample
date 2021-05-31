#ifndef __EASYPASS_H__
#define __EASYPASS_H__

#include <cstdio>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Function.h>

class EasyPass : public llvm::FunctionPass {
	public:
		static char ID;
		EasyPass() : llvm::FunctionPass(ID){}
		~EasyPass(){}

		virtual bool runOnFunction(llvm::Function &F);
		virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};

#endif
