#pragma once

#include <cassert>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

// =============================================================
// LLVM
// =============================================================

#include "llvm/Support/Error.h"
#include "llvm/Support/raw_os_ostream.h"

// =============================================================
// MLIR IR
// =============================================================

#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/TypeRange.h"
#include "mlir/IR/Types.h"
#include "mlir/IR/Value.h"
#include "mlir/IR/ValueRange.h"
#include "mlir/IR/Verifier.h"

// =============================================================
// MLIR Dialects
// =============================================================

#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/ControlFlow/IR/ControlFlow.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/SCF/IR/SCF.h"


// =============================================================
// Backend Helpers
// =============================================================

#include "backend/rex_backend_prints.h"
#include "rex_backend_exps.h"
#include "rex_backend_types.h"

// =============================================================
// AST / Types / Symbols
// =============================================================

#include "rex_ast_nodes.h"
#include "rex_exps.h"
#include "rex_scope.h"
#include "rex_stmts.h"
#include "rex_symbol.h"
#include "rex_types.h"
#include "rex_funcs.h"

namespace rex {

class IRGen {

protected:



    // =========================================================
    // CFG Helpers
    // =========================================================

    mlir::Block *currentCont() {
        assert(
            !contStack.empty() &&
            "No continuation block!"
        );

        return contStack.back();
    }

    mlir::Block *currentBreak() {
        assert(
            !breakStack.empty() &&
            "Break used outside loop!"
        );

        return breakStack.back();
    }

    // =========================================================
    // Internal Helpers
    // =========================================================

    mlir::Value i32(int value);

    bool isTerminated(mlir::Block *block);

    void branchIfNeeded(mlir::Block *target);

    mlir::Value loadIfPointer(
        mlir::Value value,
        std::shared_ptr<Type> type
    );

    mlir::Value getIterableSize(
        std::shared_ptr<Expr> expr
    );

    template<typename T>
    bool is(
        std::shared_ptr<Expr> expr,
        ExprKind kind
    );

public:

    // =========================================================
    // Scope / CFG State / Etc.
    // =========================================================

    std::shared_ptr<Scope> currentScope;

  bool blockHasTerminator(mlir::Block *block);
    std::vector<mlir::Block*> contStack;
    std::vector<mlir::Block*> breakStack;

    // =========================================================
    // Core IR State
    // =========================================================

    std::shared_ptr<mlir::OpBuilder> builder;

    mlir::ModuleOp module;
    mlir::Location loc;

    // =========================================================
    // Legacy Symbol Table
    // =========================================================
    //
    // TODO:
    // Remove once Scope fully replaces it.
    // =========================================================

    std::unordered_map<
        std::string,
        mlir::Value
    > symbolTable;

    // =========================================================
    // Function State
    // =========================================================

    mlir::func::FuncOp func_control;
    mlir::Block *exitBlock = nullptr;

    // =========================================================
    // Helpers
    // =========================================================

    std::shared_ptr<ExpressionsHelper> exps;
    std::shared_ptr<PrintHelper> prints;
    std::shared_ptr<TypesHelper> types;

    // =========================================================
    // Construction
    // =========================================================

    IRGen(
        std::shared_ptr<mlir::OpBuilder> b,
        mlir::ModuleOp &m,
        mlir::Location l,
        std::shared_ptr<TypesHelper> t
    );

    // =========================================================
    // Root
    // =========================================================
    void visitFunctionDecls(
         std::shared_ptr<FileAst> file
    );

    void visit(
        std::shared_ptr<FileAst> file
    );

    // =========================================================
    // Expressions
    // =========================================================

    mlir::Value visitExp(
        std::shared_ptr<Expr> expr
    );

    mlir::Value visitLiteral(
        std::shared_ptr<LiteralExpr> expr
    );

    mlir::Value visitBinary(
        std::shared_ptr<BinaryExpr> expr
    );

    mlir::Value visitArray(
        std::shared_ptr<ArrayExpr> expr
    );

    mlir::Value visitIndex(
        std::shared_ptr<IndexExpr> expr
    );

    mlir::Value visitTuple(
        std::shared_ptr<TupleExpr> expr
    );

    mlir::Value visitIndexTuple(
        std::shared_ptr<IndexTupleExpr> expr
    );

    mlir::Value visitId(
        std::shared_ptr<IdExpr> expr
    );

    // =========================================================
    // Statements
    // =========================================================

    void visitStmt(
        std::shared_ptr<Stmt> stmt
    );

    void visitPrint(
        std::shared_ptr<PrintStmt> stmt
    );

    void visitIf(
        std::shared_ptr<IfStmt> stmt
    );

    void visitWhile(
        std::shared_ptr<WhileStmt> stmt
    );

    void visitFor(
        std::shared_ptr<ForStmt> stmt
    );

    void visitLoop(
        std::shared_ptr<LoopStmt> stmt
    );

    void visitBlock(
        std::shared_ptr<BlockExpr> block
    );

    void visitBreak(
        std::shared_ptr<BreakStmt> stmt
    );

    // =========================================================
    // Variables / Storage
    // =========================================================

    mlir::Value allocateStorage(
        std::shared_ptr<Type> type,
        mlir::Type ptrTy,
        mlir::Value size
    );

    void initializeStorage(
        mlir::Value dst,
        mlir::Value src,
        std::shared_ptr<Type> type
    );

    mlir::Value materializeValueForStorage(
        std::shared_ptr<Expr> expr,
        mlir::Value value
    );

    void visitDelc(
        std::shared_ptr<LetStmt> stmt
    );

    void visitAssign(
        std::shared_ptr<AssignStmt> stmt
    );

    // =========================================================
    // Functions
    // =========================================================

    void visitFunctionDef(std::shared_ptr<FunctionDecl> funcDef);
    mlir::Value visitFunctionCall(std::shared_ptr<CallExpr> funcCall);
    void visitReturn(std::shared_ptr<ReturnStmt> restm);
    void visitReturnExpr(std::shared_ptr<ExprStmt> rexpstmt);
    void visitVoidCall(std::shared_ptr<ExprStmt> void_call);

};

} // namespace rex