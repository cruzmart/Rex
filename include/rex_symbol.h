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
    std::shared_ptr<Type> type;

    virtual ~Symbol() = default;

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


struct VariableSymbol : Symbol {
    mlir::Value ptr; // ALWAYS pointer


    VariableSymbol(std::string name,
                   std::shared_ptr<Type> type,
                   mlir::Value ptr)
        : Symbol(SymbolType::Variable, std::move(name)) {
        this->type = type;
        this->ptr = ptr;
    }
    VariableSymbol(std::string name)
        : Symbol(SymbolType::Variable, std::move(name)) {
        this->type = nullptr;
        this->ptr = nullptr;
    }
};


struct FunctionSymbol : Symbol {
    mlir::LLVM::LLVMFuncOp func; // function reference

    FunctionSymbol(std::string name,
                   std::shared_ptr<Type> type,
                   mlir::LLVM::LLVMFuncOp func)
        : Symbol(SymbolType::Function, std::move(name)) {
        this->type = type;
        this->func = func;
    }

    FunctionSymbol(std::string name)
        : Symbol(SymbolType::Function, std::move(name)) {
        this->type = nullptr;
        this->func = nullptr;
    }
};


struct TypeSymbol : Symbol {
    std::shared_ptr<Type> aliased;

    TypeSymbol(std::string name,
               std::shared_ptr<Type> aliased)
        : Symbol(SymbolType::Typealias, std::move(name)) {
        this->aliased = aliased;
        this->type = aliased; // optional but usually useful
    }
};

} // namespace rex