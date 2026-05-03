#pragma once

// Pass manager
#include <memory>
#include <string_view>
#include <strings.h>


#include "mlir/Conversion/ArithToLLVM/ArithToLLVM.h"
#include "mlir/Conversion/ControlFlowToLLVM/ControlFlowToLLVM.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVM.h"
#include "mlir/Conversion/FuncToLLVM/ConvertFuncToLLVMPass.h"
#include "mlir/Conversion/MemRefToLLVM/MemRefToLLVM.h"
#include "mlir/Conversion/ReconcileUnrealizedCasts/ReconcileUnrealizedCasts.h"
#include "mlir/Conversion/SCFToControlFlow/SCFToControlFlow.h"
#include "mlir/IR/Types.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Pass/PassManager.h"

// Translation
#include "llvm/Support/raw_os_ostream.h"
#include "mlir/Target/LLVMIR/Dialect/Builtin/BuiltinToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Dialect/LLVMIR/LLVMToLLVMIRTranslation.h"
#include "mlir/Target/LLVMIR/Export.h"

// MLIR IR
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/TypeRange.h"
#include "mlir/IR/Value.h"
#include "mlir/IR/ValueRange.h"
#include "mlir/IR/Verifier.h"

// Dialects
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/ControlFlow/IR/ControlFlow.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/Dialect/MemRef/IR/MemRef.h"
#include "mlir/Dialect/SCF/IR/SCF.h"


// Errors
#include "llvm/Support/Error.h"


#include "mlir/IR/Builders.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/BuiltinOps.h"


#include "backend/rex_backend_prints.h"
#include "rex_backend_exps.h"
#include "rex_backend_types.h"
#include "rex_stmts.h"
#include "rex_types.h"
#include "rex_exps.h"
#include "rex_ast_nodes.h"
#include "rex_stmts.h"
#include "rex_symbol.h"
#include "rex_scope.h"

namespace rex {

    class IRGen {

    private:
        mlir::Block *contBlock;
        mlir::Block* currentCont() {
            assert(!contStack.empty() && "No continuation block!");
            return contStack.back();
        }
        mlir::Block* currentBreak() {
            assert(!breakStack.empty() && "break used outside of loop!");
            return breakStack.back();
        }
        std::shared_ptr<Scope> currentScope;
        mlir::Value getIterableSize(std::shared_ptr<Expr> exp);
        
                
        
    public:
        std::shared_ptr<mlir::OpBuilder> builder;
        mlir::ModuleOp module;
        mlir::Location loc;
        std::unordered_map<std::string, mlir::Value> symbolTable;
        mlir::func::FuncOp func_control;
        mlir::Block * exitBlock;
        bool blockHasTerminator(mlir::Block *block);

   

        std::shared_ptr<ExpressionsHelper> exps;
        std::shared_ptr<PrintHelper> prints;
        std::shared_ptr<TypesHelper> types;

        IRGen(std::shared_ptr<mlir::OpBuilder> b,
            mlir::ModuleOp &m,
            mlir::Location l,
            std::shared_ptr<TypesHelper> t);


        // CFG Helpers
        std::vector<mlir::Block*> contStack;
        std::vector<mlir::Block*> breakStack;


        mlir::Value visitExp(std::shared_ptr<Expr> expr);
        mlir::Value visitLiteral(std::shared_ptr<LiteralExpr> l);
        mlir::Value visitBinary(std::shared_ptr<BinaryExpr> bi);
        mlir::Value visitArray(std::shared_ptr<ArrayExpr> arr);
        mlir::Value visitIndex(std::shared_ptr<IndexExpr> i);
        mlir::Value visitTuple(std::shared_ptr<TupleExpr> tup);
        mlir::Value visitIndexTuple(std::shared_ptr<IndexTupleExpr> it);


        void visitPrint(std::shared_ptr<PrintStmt> p);
        void visit(std::shared_ptr<FileAst> file);

        // Variable
        mlir::Value visitId(std::shared_ptr<IdExpr> id);


        // Variable Decleration
        void visitDelc(std::shared_ptr<LetStmt> var);


        void visitAssign(std::shared_ptr<AssignStmt> var);
        void visitAssignIndex(std::shared_ptr<IndexExpr> target, mlir::Value value);
    

        // Flow Control Functions
        void visitStmt(std::shared_ptr<Stmt> stmt);
        void visitIf(std::shared_ptr<IfStmt> if_stmt);
        void visitWhile(std::shared_ptr<WhileStmt> whle_stmt);
        void visitFor(std::shared_ptr<ForStmt> for_stmt);
        void visitLoop(std::shared_ptr<LoopStmt> lop_stmt);
        void visitBlock(std::shared_ptr<BlockExpr> block);
        void visitBreak(std::shared_ptr<BreakStmt> brk);
    };
}