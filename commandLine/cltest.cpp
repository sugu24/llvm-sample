#include "llvm/Support/CommandLine.h"
#include <cstdio>

namespace OutputFileName{
	llvm::cl::opt<std::string> OutputFile(
			"o",
			llvm::cl::desc("出力ファイルを指定します"),
			llvm::cl::value_desc("filename"),
			llvm::cl::init("-")
	);
}

namespace InputFileNames{
	llvm::cl::list<std::string> InputFiles(
			llvm::cl::Positional,
			llvm::cl::desc("<input file>"),
			llvm::cl::OneOrMore
	);
}

namespace JIT{
	llvm::cl::opt<bool> WithJit(
			"jit",
			llvm::cl::desc("JIT コンパイルを有効にします"),
			llvm::cl::init(false) // -jitが指定されたら勝手にtrueになる
	);
}

namespace Optimization{
	enum OptLevel{ NONE, O1, O2, O3 };
	
	// オプション定義
	llvm::cl::opt<OptLevel> OLevel(
		llvm::cl::values(
			clEnumValN( NONE, "O0", "最適化しません"),
			clEnumVal( O1, "最適化しません"),
			clEnumVal( O2, "基本的な最適化"),
			clEnumVal( O3, "さらに積極的な最適化"),
		        clEnumValEnd),
		llvm::cl::desc("最適化レベルの選択")
	);
}

/**
 * main関数
 */
int main(int argc, char **argv){
	// 引数のパース
	llvm::cl::ParseCommandLineOptions(argc, argv, "CommandLine Sample\n");

	// InputFileの取得
	for (int i = 0; i < InputFileNames::InputFiles.size(); i++)
		fprintf(stderr, "input file : %s\n", InputFileNames::InputFiles[i].c_str());

	// OutputFileの取得
	fprintf(stderr, "output file : %s\n", OutputFileName::OutputFile.c_str());

	if(JIT::WithJit)
	       	fprintf(stderr, "enable jit\n");
	else
		fprintf(stderr, "unable jit\n");
	
	fprintf(stderr, "Optimization Level : %d\n", (int) Optimization::OLevel);

	return 0;
}
