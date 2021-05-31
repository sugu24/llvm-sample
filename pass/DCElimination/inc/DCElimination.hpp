#ifndef __DCELIMINATION_H__
#define __DCELIMINATION_H__

#include <cstdio>
#include <list>
#include <llvm/Function.h>
#include <llvm/Instructions.h>
#include <llvm/Constants.h>
#include <llvm/Operator.h>
#include <llvm/Pass.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/InstIterator.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/LoopPass.h>

class DCEliminationPass : public llvm::FunctionPass{
	public:
		static char ID;
		DCEliminationPass() : llvm::FunctionPass(ID){}
		~DCEliminationPass(){}

		virtual bool runOnFunction(llvm::Function &F);
		virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};

#endif
