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

namespace rex {

    class IRGen {

    public:
        std::shared_ptr<mlir::OpBuilder> builder;
        mlir::ModuleOp module;
        mlir::Location loc;
        std::unordered_map<std::string, mlir::Value> symbolTable;
        mlir::func::FuncOp func_control;


        std::shared_ptr<ExpressionsHelper> exps;
        std::shared_ptr<PrintHelper> prints;

        IRGen(std::shared_ptr<mlir::OpBuilder> b,
            mlir::ModuleOp &m,
            mlir::Location l);

        mlir::Value visitExp(std::shared_ptr<Expr> expr);
        mlir::Value visitLiteral(std::shared_ptr<LiteralExpr> l);
        mlir::Value visitBinary(std::shared_ptr<BinaryExpr> bi);
        mlir::Value visitArray(std::shared_ptr<ArrayExpr> arr);
        mlir::Value visitIndex(std::shared_ptr<IndexExpr> i);
        mlir::Value visitTuple(std::shared_ptr<TupleExpr> tup);
        mlir::Value visitIndexTuple(std::shared_ptr<IndexTupleExpr> it);


        void visitPrint(std::shared_ptr<PrintStmt> p);

        void visitStmt(std::shared_ptr<Stmt> stmt);
        void visit(std::shared_ptr<FileAst> file);


        // Flow Control Functions
        void visitIf(std::shared_ptr<IfStmt> if_stmt);
        void visitBlock(std::shared_ptr<BlockExpr> block);
    };
}