#include "passes/rex_alias_pass.h"
#include "rex_stmts.h"
#include <memory>
#include <stdexcept>
#include <iostream>

namespace rex {

// ------------------- Entry Point -------------------
void AliasPass::visit(const std::shared_ptr<FileAst> file) {
    for(auto node: file->items){
        if(auto stmt = std::dynamic_pointer_cast<Stmt>(node)){
            visitStmt(stmt);
        }
    }

}

// ------------------- Visit Statements -------------------
void AliasPass::visitStmt(const std::shared_ptr<Stmt> stmt) {
 

    if (auto let_stmt = std::dynamic_pointer_cast<LetStmt>(stmt)) {
        visitLetStmt(let_stmt);
    }
    if (auto while_stmt = std::dynamic_pointer_cast<WhileStmt>(stmt)) {
        visitBlock(while_stmt->body);
    } else if (auto for_stmt = std::dynamic_pointer_cast<ForStmt>(stmt)) {
        visitBlock(for_stmt->body);
    } else if (auto loop_stmt = std::dynamic_pointer_cast<LoopStmt>(stmt)) {
        visitBlock(loop_stmt->body);
    } else if (auto if_stmt = std::dynamic_pointer_cast<IfStmt>(stmt)) {
        visitBlock(if_stmt->then_block);
        for (auto& [cond, block] : if_stmt->elifx_blocks) {
            visitBlock(block);
        }
        if (if_stmt->else_block)
            visitBlock(if_stmt->else_block);
    } else if (auto td = std::dynamic_pointer_cast<TypeDecl>(stmt)){
        visitTypeDecl(td);
    }
}

void AliasPass::visitLetStmt(const std::shared_ptr<LetStmt> stmt) {
    if (!stmt->type) return; // nothing to resolve

    stmt->type = resolveType(stmt->type); // resolve NamedType recursively

    // Optionally, you could also check if the resolved type exists in the scope
    // and throw an error if not, but resolveType already does that
}

// ------------------- Visit Block -------------------
void AliasPass::visitBlock(const std::shared_ptr<BlockExpr> block) {
    // Push a new scope for this block
    auto previous_scope = current_scope;
    current_scope = current_scope->push();

    // Visit statements inside the block
    for (auto& stmt : block->statements) {
        visitStmt(stmt);
    }

    current_scope = previous_scope; // pop scope
}

// ------------------- Visit Type Declaration -------------------
void AliasPass::visitTypeDecl(const std::shared_ptr<TypeDecl> td) {
    if (!td->type) {
        throw std::runtime_error("TypeDecl has null type for " + td->name);
    }

    // Resolve NamedType inside the type expression
    td->type = resolveType(td->type);

    // Add type alias to current scope
    auto symbol = std::make_shared<Symbol>(SymbolType::typealias, td->name);
    symbol->type = td->type;

    if (current_scope->symbols.find(td->name) != current_scope->symbols.end()) {
        throw std::runtime_error("Type alias '" + td->name + "' already defined in this scope");
    }

    current_scope->define(symbol);
}

// ------------------- Resolve NamedType -------------------
std::shared_ptr<Type> AliasPass::resolveType(const std::shared_ptr<Type>& type) {
    if (auto named = std::dynamic_pointer_cast<NamedType>(type)) {
        auto sym = current_scope->resolve(named->alias);
        if (!sym) {
            throw std::runtime_error("Unknown type alias: " + named->alias);
        }
        // Recursively resolve in case the alias points to another alias
        return resolveType(sym->type);
    }
    else if (auto arr = std::dynamic_pointer_cast<ArrayType>(type)) {
        arr->array_type = resolveType(arr->array_type);
        return arr;
    }
    else if (auto slice = std::dynamic_pointer_cast<SliceType>(type)) {
        slice->slice_type = resolveType(slice->slice_type);
        return slice;
    }
    else if (auto rng = std::dynamic_pointer_cast<RangeType>(type)) {
        rng->lower_type = resolveType(rng->lower_type);
        rng->upper_type = resolveType(rng->upper_type);
        return rng;
    }
    else if (auto tup = std::dynamic_pointer_cast<TupleType>(type)) {
        for (auto& elem : tup->tuple_types) {
            elem = resolveType(elem);
        }
        return tup;
    }
    else if (auto pipe = std::dynamic_pointer_cast<PipeType>(type)) {
        pipe->func_left_type = resolveType(pipe->func_left_type);
        pipe->func_right_type = resolveType(pipe->func_right_type);
        return pipe;
    }
    else if (auto fn = std::dynamic_pointer_cast<FunctionType>(type)) {
        for (auto& param : fn->params_type) {
            param = resolveType(param);
        }
        fn->return_type = resolveType(fn->return_type);
        return fn;
    }

    // For PrimType, NamedType already handled, everything else return as-is
    return type;
}

} // namespace rex