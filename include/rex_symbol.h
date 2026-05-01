#pragma once
#include <memory>
#include <string>
#include "backend/rex_backend_exps.h"
#include "backend/rex_backend_prints.h"
#include "backend/rex_backend_types.h"
#include "rex_exps.h"
#include "rex_ops.h"
#include "rex_types.h"
#include <memory>
#include <mlir/Conversion/ArithToLLVM/ArithToLLVM.h>
#include <mlir/Dialect/Arith/IR/Arith.h>
#include <mlir/Dialect/LLVMIR/LLVMDialect.h>
#include <mlir/Dialect/LLVMIR/LLVMTypes.h>
#include <mlir/IR/Builders.h>
#include <mlir/IR/BuiltinAttributes.h>
#include <mlir/IR/ValueRange.h>



namespace rex {

struct Type;
struct Expr;

// ------------------ SYMBOL KIND -------------------

enum class SymbolType {
    Variable,
    Function,
    Typealias
};

// ------------------ SYMBOL -----------------------

struct Symbol {
    SymbolType kind;
    std::string name;

    // type: shared_type information
    std::shared_ptr<Type> type;

    // optional expression (initializer or function body)
    std::shared_ptr<Expr> expr;

    // mlir::LLVM::AllocaOp value;
    mlir::Value value;

    Symbol(SymbolType k, std::string n)
        : kind(k), name(std::move(n)) {}

    std::string sym_type(){
        switch(kind){
            case rex::SymbolType::Function:
                return "function";
            case rex::SymbolType::Variable:
                return "variable";
            case rex::SymbolType::Typealias:
                return "typealias";
            default:
                return "<?>";
        }
    }
};

} // namespace rex