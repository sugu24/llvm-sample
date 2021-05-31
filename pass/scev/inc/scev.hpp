#ifndef _SCEV_H__
#define _SCEV_H__

#include <cstdio>
#include <llvm/Analysis/LoopPass.h>
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/ScalarEvolution.h>
#include <llvm/Constants.h>
#include <llvm/Instructions.h>
#include <llvm/Operator.h>
#include <llvm/Function.h>
#include <llvm/Pass.h>
#include <llvm/Support/Casting.h>

class SCEV : public llvm::LoopPass{
	public:
		static char ID;
		SCEV() : llvm::LoopPass(ID){}
		~SCEV(){}

		virtual bool runOnLoop(llvm::Loop *L, llvm::LPPassManager &LPM);
		virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};

#endif
