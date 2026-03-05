#include "passes/rex_alias_pass.h"
#include "rex_funcs.h"
#include "rex_scope.h"
#include "rex_stmts.h"
#include <memory>
#include <stdexcept>

namespace rex {

// -------------------- Helper Function ------------------

void AliasPass::print(const std::string& msg) const {
    if (debug)
        std::cout << "[AliasPass] " << msg << "\n";

}

// ------------------- Entry Point -------------------
void AliasPass::visit(const std::shared_ptr<FileAst> file) {
    print("Enter global Scope\n");
    for (auto node : file->items) {
        if (auto stmt = std::dynamic_pointer_cast<Stmt>(node)) {
            visitStmt(stmt);
        } 
        // else if (auto func = std::dynamic_pointer_cast<FunctionDecl>(node)){
        //     visitFunctionDecl(func);
        // }
    }
    print("Exit Global Scope\n");
}

// ------------------- Visit Functions -------------------
// ------------------- Visit Statements -------------------
void AliasPass::visitStmt(const std::shared_ptr<Stmt> stmt) {

    // ---- SIMPLE DECLARATIONS ----
    // if (auto let_stmt = std::dynamic_pointer_cast<LetStmt>(stmt)) {
    //     visitLetStmt(let_stmt);
    //     return;
    // }

    if (auto type_decl = std::dynamic_pointer_cast<TypeDecl>(stmt)) {
        visitTypeDecl(type_decl);
        return;
    }

    // ---- CONTROL FLOW (with block scopes) ----
    if (auto while_stmt = std::dynamic_pointer_cast<WhileStmt>(stmt)) {
        visitBlock(while_stmt->body);
        return;
    }

    if (auto for_stmt = std::dynamic_pointer_cast<ForStmt>(stmt)) {
        visitBlock(for_stmt->body);
        return;
    }

    if (auto loop_stmt = std::dynamic_pointer_cast<LoopStmt>(stmt)) {
        visitBlock(loop_stmt->body);
        return;
    }

    if (auto if_stmt = std::dynamic_pointer_cast<IfStmt>(stmt)) {
        visitBlock(if_stmt->then_block);

        for (auto& [cond, block] : if_stmt->elifx_blocks)
            visitBlock(block);

        if (if_stmt->else_block)
            visitBlock(if_stmt->else_block);

        return;
    }

    // If we reach here, it's a stmt that doesn't matter for alias pass
}

// ------------------- Visit Block -------------------
void AliasPass::visitBlock(const std::shared_ptr<BlockExpr> block) {
    if (!block) return;


    print("Entering Scope Depth: " + std::to_string(scope_depth) + "\n");
    auto prev = current_scope;
    current_scope = current_scope->push();
    scope_depth += 1;

    for (auto& stmt : block->statements)
        visitStmt(stmt);

    current_scope = prev; // pop
    print("Leaving Scope Depth: " + std::to_string(scope_depth) + "\n");
    scope_depth -= 1;
}

// ------------------- LetStmt -------------------
void AliasPass::visitLetStmt(const std::shared_ptr<LetStmt> ls) {
  return;
}

// ------------------- TypeDecl -------------------
void AliasPass::visitTypeDecl(const std::shared_ptr<TypeDecl> td) {
    // if (!td->type)
    //     throw std::runtime_error("TypeDecl null type: " + td->name);

    // td->type = resolveType(td->type);

    // auto sym = std::make_shared<Symbol>(SymbolType::typealias, td->name);
    // sym->type = td->type;

    // if (current_scope->symbols.contains(td->name))
    //     throw std::runtime_error("Type alias '" + td->name + "' already defined");


    // current_scope->define(sym);
    // td->resolved = sym;


       if (!td->type)
        throw std::runtime_error("TypeDecl null type: " + td->name);

    print("Resolving type alias: " + td->name +
               " = " + td->type->to_string());

    td->type = resolveType(td->type);

    print("Resolved alias '" + td->name +
               "' → " + td->type->to_string());

    auto sym = std::make_shared<Symbol>(SymbolType::typealias, td->name);
    sym->type = td->type;

    if (current_scope->symbols.contains(td->name))
        throw std::runtime_error("Type alias '" + td->name + "' already defined");

    current_scope->define(sym);
    td->resolved = sym;

    print("Alias stored in scope: " + td->name);
}

// ------------------- FunctionDecl -------------------
void AliasPass::visitFunctionDecl(const std::shared_ptr<FunctionDecl> fn) {
    return;
}

// ------------------- Resolve Types -------------------
std::shared_ptr<Type> AliasPass::resolveType(const std::shared_ptr<Type>& type) {

     print("Resolving type node: " + type->to_string());

    if (auto named = std::dynamic_pointer_cast<NamedType>(type)) {
        print("NamedType: alias = " + named->alias);

        if (named->actual_type) {
            print("Alias already resolved → " + named->actual_type->to_string());
            return named->actual_type;
        }

        auto sym = current_scope->resolve(named->alias);

        if (!sym)
            throw std::runtime_error("Unknown type alias: " + named->alias);

        print("Alias refers to → " + sym->type->to_string());

        named->actual_type = resolveType(sym->type);

        print("Alias final resolution: " + named->actual_type->to_string());
        return named->actual_type;
    }

    if (auto arr = std::dynamic_pointer_cast<ArrayType>(type)) {
        print("Resolving ArrayType: [" + arr->array_type->to_string() + "]");
        arr->array_type = resolveType(arr->array_type);
        return arr;
    }

    if (auto slice = std::dynamic_pointer_cast<SliceType>(type)) {
        print("Resolving SliceType: []" + slice->slice_type->to_string());
        slice->slice_type = resolveType(slice->slice_type);
        return slice;
    }

    if (auto rng = std::dynamic_pointer_cast<RangeType>(type)) {
        print("Resolving RangeType");
        rng->lower_type = resolveType(rng->lower_type);
        rng->upper_type = resolveType(rng->upper_type);
        return rng;
    }

    if (auto tup = std::dynamic_pointer_cast<TupleType>(type)) {
        print("Resolving TupleType of size " + std::to_string(tup->tuple_types.size()));
        for (auto& t : tup->tuple_types)
            t = resolveType(t);
        return tup;
    }

    if (auto fn = std::dynamic_pointer_cast<FunctionType>(type)) {
        print("Resolving FunctionType");
        for (auto& p : fn->params_type)
            p = resolveType(p);
        fn->return_type = resolveType(fn->return_type);
        return fn;
    }

    // Primitive or already resolved
    print("Primitive or simple type: " + type->to_string());
    return type;

}

} // namespace rex