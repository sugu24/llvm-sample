#ifndef AST_HPP
#define AST_HPP

#include <string>
#include <map>
#include <vector>
#include <llvm/Support/Casting.h>
#include "APP.hpp"

/*
 * クラス宣言
 */
class BaseAST;
class TranslationUnitAST;
class FunctionAST;
class PrototypeAST;
class FunctionStmtAST;
class VariableDeclAST;
class BinaryExprAST;
class NullExprAST;
class CallExprAST;
class JumpStmtAST;
class VariableAST;
class NumberAST;


/**
 * ASTの種類
 */
enum AstID{
	BaseID,
	VariableDeclID,
	BinaryExprID,
	NullExprID,
	CallExprID,
	JumpStmtID,
	VariableID,
	NumberID
};

/**
 * ASTの基底クラス
 */
class BaseAST{
	AstID ID;

	public:
	BaseAST(AstID id):ID(id){}
	virtual ~BaseAST(){}
	AstID getValueID() const {return ID;}
};

/**
 * ソースコードを表すAST
 */
class TranslationUnitAST{
	std::vector<PrototypeAST*> Prototypes;
	std::vector<FunctionAST*> Functions;

	public:
		TranslationUnitAST(){}
		~TranslationUnitAST();

		// モジュールにプロトタイプ宣言を追加する
		bool addPrototype (PrototypeAST *proto);

		// モジュールに関数を追加する
		bool addFunction(FunctionAST *func);

		// モジュールがからか判定する
		bool empty();

		// i番目のプロトタイプ宣言を取得する
		PrototypeAST *getPrototype(int i){
			if (i < Prototypes.size())
				return Prototypes.at(i);
			else
				return NULL;
		}

		// i番目の関数を取得する
		FunctionAST *getFunction(int i){
			if(i < Functions.size())
				return Functions.at(i);
			else
				return NULL;
		}
};

/**
 * 関数宣言を表すAST
 */
class PrototypeAST{
	std::string Name;
	std::vector<std::string> Params;

	public:
		PrototypeAST (const std::string &name, const std::vector<std::string> &params)
			: Name(name), Params(params){}

		// 関数名を取得する
		std::string getName(){return Name;}

		// i番目の引数名を取得する
		std::string getParamName(int i){if (i<Params.size()) return Params.at(i); return NULL;}

		// 引数の数を取得sる
		int getParamNum(){return Params.size();}
};

/**
 * 関数定義を表すAST
 */
class FunctionAST{
	PrototypeAST *Proto;
	FunctionStmtAST *Body;
	
	public:
	FunctionAST(PrototypeAST *proto, FunctionStmtAST *body) : Proto(proto), Body(body){}
	~FunctionAST();

	// 関数名を取得する
	std::string getName(){return Proto->getName();}

	// この関数のプロトタイプ宣言を取得する
	PrototypeAST *getPrototype(){return Proto;}

	// この関数のボディを取得する
	FunctionStmtAST *getBody(){return Body;}
};

/**
 * 関数定義（ボディ）を表すAST
 */
class FunctionStmtAST{
	std::vector<VariableDeclAST*> VariableDecls;
	std::vector<BaseAST*> StmtLists;

	public:
	FunctionStmtAST(){}
	~FunctionStmtAST();

	// 関数に変数を追加する
	bool addVariableDeclaration(VariableDeclAST *vdecl);

	// 関数にステートメントを追加する
	bool addStatement(BaseAST *stmt){StmtLists.push_back(stmt); return true;}

	// i番目の変数を取得する
	VariableDeclAST *getVariableDecl(int i){if(i<VariableDecls.size()) return VariableDecls.at(i); else return NULL;}

	// i番目のステートメントを取得する
	BaseAST *getStatement(int i){if(i<StmtLists.size()) return StmtLists.at(i); else return NULL;}
};

/**
 * 変数宣言を表すAST
 */
class VariableDeclAST: public BaseAST{
	public:
		typedef enum{
			param,
			local
		}DeclType;
	
	private:
		std::string Name;
		DeclType Type;

	public:
		VariableDeclAST(const std::string &name) : BaseAST(VariableDeclID),Name(name){
		}

		// VariableDeclASTなのでtrueを返す
		static inline bool classof (VariableDeclAST const*){return true;}

		// 渡されたBaseASTクラスがVarialbeDeclASTか判定する
		static inline bool classof(BaseAST const* base){
			return base->getValueID() == VariableDeclID;
		}
		~VariableDeclAST(){}

		// 変数名を取得する
		std::string getName(){return Name;}

		//変数の宣言種別を設定する
		bool setDeclType(DeclType type){Type = type; return true;}

		// 変数の宣下種別を設定する
		DeclType getType(){return Type;}
};

/**
 * 二幸演算を表すAST
 */
class BinaryExprAST : public BaseAST{
	std::string Op;
	BaseAST *LHS, *RHS;

	public:
		BinaryExprAST(std::string op, BaseAST *lhs, BaseAST *rhs) 
			: BaseAST(BinaryExprID),Op(op),LHS(lhs),RHS(rhs){}
		~BinaryExprAST(){SAFE_DELETE(LHS);SAFE_DELETE(RHS);}

		// BinaryExprASTなのでtrueを返す
		static inline bool classof(BinaryExprAST const* base){return true;}

		// 渡されたBaseASTがBinaryExprASTか判定する
		static inline bool classof(BaseAST const* base){
			return base->getValueID() == BinaryExprID;
		}

		// 演算子を取得する
		std::string getOp(){return Op;}

		// 左辺値を取得
		BaseAST *getLHS(){return LHS;}

		// 右辺値を取得
		BaseAST *getRHS(){return RHS;}
};

/**
 * 関数呼び出しを表すAST
 */
class CallExprAST : public BaseAST{
	std::string Callee;
	std::vector<BaseAST*> Args;

	public:
		CallExprAST(const std::string &callee, std::vector<BaseAST*> &args)
			: BaseAST(CallExprID),Callee(callee),Args(args){}
		~CallExprAST();

		// CallExprASTなのでtrueを返す
		static inline bool classof (CallExprAST const*){return true;}

		// 渡されたBaseASTがCallExprASTか判定する
		static inline bool classof(BaseAST const* base){
			return base->getValueID() == CallExprID;
		}

		// 呼び出す関数名を取得する
		std::string getCallee(){return Callee;}

		// i番目の引数を取得する
		BaseAST *getArgs (int i){if(i<Args.size())return Args.at(i);else return NULL;}
};

/**
 * ジャンプ(ここではreturn)を表すAST
 * (returnの変数を扱う？)
 */
class JumpStmtAST : public BaseAST {
	BaseAST *Expr;
	public:
		JumpStmtAST(BaseAST *expr) : BaseAST(JumpStmtID),Expr(expr){}
		~JumpStmtAST(){SAFE_DELETE(Expr);}

		// JumpSgmgASTなのでtrueを返す
		static inline bool classof(JumpStmtAST const*){return true;}

		// 渡されたBaseASTがJumpStmtASTか判定する
		static inline bool classof(BaseAST const* base){
			return base->getValueID() == JumpStmtID;
		}

		// returnで返すExpressionを取得する
		BaseAST *getExpr(){return Expr;}
};

/**
 * 変数参照を表すAST
 */
class VariableAST : public BaseAST{
	//Name
	std::string Name;

	public:
		VariableAST(const std::string &name) : BaseAST(VariableID),Name(name){}
		~VariableAST(){}

		// VariableASTなのでtrueを返す
		static inline bool classof(VariableAST const*){return true;}

		// 渡されたBaseAstがVariableASTか判定する
		static inline bool classof(BaseAST const* base){
			return base->getValueID() == VariableID;
		}

		// 変数名を取得
		std::string getName(){return Name;}
};

/**
 * 変数を表すAST
 */
class NumberAST : public BaseAST{
	int Val;
	
	public:
		NumberAST(int val) : BaseAST(NumberID), Val(val){}
		~NumberAST(){}

		// NumberASTなのでtrueを返す
		static inline bool classof(NumberAST const*){return true;}

		//渡されたBaseASTがNumberASTか判定する
		static inline bool classof(BaseAST const* base){
			return base->getValueID() == NumberID;
		}

		// このASTが表現する数値を取得する
		int getNumberValue(){return Val;}
};

/**
 * ";"を表すAST
 */
class NullExprAST : public BaseAST {
	public:
		NullExprAST() : BaseAST(NullExprID){}
		static inline bool classof(NullExprAST const*){return true;}
		static inline bool classof(BaseAST const* base){
			return base->getValueID() == NullExprID;
		}
};

#endif
