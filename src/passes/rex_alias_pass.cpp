#include "passes/rex_alias_pass.h"
#include <cassert>
#include <stdexcept>
#include <iostream>

namespace rex {

// -------------------- Helper --------------------
void AliasPass::print(const std::string& msg) const {
    if (debug) std::cout << "[AliasPass] " << msg << "\n";
}

// -------------------- Entry --------------------
void AliasPass::visit(const std::shared_ptr<FileAst>& file) {
   print("Enter global Scope");

    for (auto& node : file->items) {
        switch (node->ast_kind) {
            case AstNodeKind::Stmt:
                visitStmt(std::static_pointer_cast<Stmt>(node));
                break;
            case AstNodeKind::FunctionDecl:
                visitFunctionDecl(std::static_pointer_cast<FunctionDecl>(node));
                break;
            default:
                break; // ignore
        }
    }

    print("Exit global Scope");
}

// -------------------- Statements --------------------
void AliasPass::visitStmt(const std::shared_ptr<Stmt>& stmt) {

    switch (stmt->stmt_kind) {

        case StmtKind::LetDecl: {
            auto ls = std::static_pointer_cast<LetStmt>(stmt);
            visitLetStmt(ls);
            break;
        }

        case StmtKind::TypeDecl: {
            auto td = std::static_pointer_cast<TypeDecl>(stmt);
            visitTypeDecl(td);
            break;
        }

        case StmtKind::While: {
            auto w = std::static_pointer_cast<WhileStmt>(stmt);
            visitBlock(w->body);
            break;
        }

        case StmtKind::For: {
            auto f = std::static_pointer_cast<ForStmt>(stmt);
            visitBlock(f->body);
            break;
        }

        case StmtKind::Loop: {
            auto l = std::static_pointer_cast<LoopStmt>(stmt);
            visitBlock(l->body);
            break;
        }

        case StmtKind::If: {
            auto i = std::static_pointer_cast<IfStmt>(stmt);
            visitBlock(i->then_block);
            for (auto& [cond, blk] : i->elifx_blocks)
                visitBlock(blk);
            if (i->else_block) visitBlock(i->else_block);
            break;
        }

        default:
            break; // other statements ignored
    }
}

// -------------------- Blocks --------------------
void AliasPass::visitBlock(const std::shared_ptr<BlockExpr>& block) {
    if (!block) return;

    print("Entering Scope Depth: " + std::to_string(scope_depth));
    auto prev = current_scope;
    current_scope = current_scope->push();
    scope_depth++;

    for (auto& stmt : block->statements)
        visitStmt(stmt);

    current_scope = prev;
    scope_depth--;
    print("Leaving Scope Depth: " + std::to_string(scope_depth));
}

// -------------------- LetStmt --------------------
void AliasPass::visitLetStmt(const std::shared_ptr<LetStmt>& ls) {
   if (!ls->type) return;

    assert(ls->id_pattern && "LetStmt must have a pattern");

    print("Resolving let type: " + ls->id_pattern->to_string() +
          " = " + ls->type->to_fundamental_string());

    ls->type = resolveType(ls->type);

    print("Resolved let type → " + ls->type->to_string());
}

// -------------------- TypeDecl --------------------
void AliasPass::visitTypeDecl(const std::shared_ptr<TypeDecl>& td) {
  if (!td->type) throw std::runtime_error("TypeDecl null type: " + td->name);

    print("Resolving type alias: " + td->name + " = " + td->type->to_string());

    td->type = resolveType(td->type);

    auto sym = std::make_shared<Symbol>(SymbolType::Typealias, td->name);
    sym->type = td->type;

    if (current_scope->symbols.contains(td->name))
        throw std::runtime_error("Type alias '" + td->name + "' already defined");

    current_scope->define(sym);
    td->resolved = sym;

    print("Alias stored in scope: " + td->name);
}

// -------------------- FunctionDecl --------------------
void AliasPass::visitFunctionDecl(const std::shared_ptr<FunctionDecl>& fn) {

       print("Visiting function: " + fn->func_name);

    for (auto& param : fn->func_type->params) {
        if (!param->para_type) continue;

        print("Resolving param '" + param->para_name + "' type: " +
              param->para_type->to_string());

        param->para_type = resolveType(param->para_type);

        print("Resolved param '" + param->para_name + "' → " +
              param->para_type->to_string());
    }

    if (fn->func_type->ret) {
        print("Resolving return type: " + fn->func_type->ret->to_string());
        fn->func_type->ret = resolveType(fn->func_type->ret);
        print("Resolved return type → " + fn->func_type->ret->to_string());
    }

    print("Entering function scope: " + fn->func_name);
    auto prev = current_scope;
    current_scope = current_scope->push();
    scope_depth++;

    visitBlock(fn->body);

    current_scope = prev;
    scope_depth--;
    print("Leaving function scope: " + fn->func_name);
}

// -------------------- Type Resolution --------------------
std::shared_ptr<Type> AliasPass::resolveType(const std::shared_ptr<Type>& type) {

       assert(type && "resolveType called with null type");
    print("Resolving type node: " + type->to_string());


    switch(type->kind) {

        case TypeKind::Named: {
            auto named = std::static_pointer_cast<NamedType>(type);
            assert(named && "NamedType cast failed");

            print("NamedType alias: " + named->alias);

            if (named->actual) return named->actual;

            auto sym = current_scope->resolve(named->alias);

            if (!sym)
                throw std::runtime_error("Unknown type alias: " + named->alias);

            named->actual = resolveType(sym->type);
            print("Alias final resolution: " + named->actual->to_string());
            return named->actual;
        }

        case TypeKind::Array: {
            auto arr = std::static_pointer_cast<ArrayType>(type);
            assert(arr && "ArrayType cast failed");
            arr->elem = resolveType(arr->elem);
            return arr;
        }

        case TypeKind::Slice: {
            auto slice = std::static_pointer_cast<SliceType>(type);
            assert(slice && "SliceType cast failed");
            slice->elem = resolveType(slice->elem);
            return slice;
        }

        case TypeKind::Tuple: {
            auto tup = std::static_pointer_cast<TupleType>(type);
            assert(tup && "TupleType cast failed");
            for(auto& e : tup->elements)
                e = resolveType(e);
            return tup;
        }

        case TypeKind::Range: {
            auto rng = std::static_pointer_cast<RangeType>(type);
            assert(rng && "RangeType cast failed");
            rng->lower = resolveType(rng->lower);
            rng->upper = resolveType(rng->upper);
            return rng;
        }

        case TypeKind::Pipe: {
            auto pipe = std::static_pointer_cast<PipeType>(type);
            assert(pipe && "PipeType cast failed");
            pipe->left  = resolveType(pipe->left);
            pipe->right = resolveType(pipe->right);
            return pipe;
        }

        case TypeKind::Function: {
            auto fn = std::static_pointer_cast<FunctionType>(type);
            assert(fn && "FunctionType cast failed");
            for(auto& p : fn->params)
                p->para_type = resolveType(p->para_type);
            fn->ret = resolveType(fn->ret);
            return fn;
        }

        case TypeKind::Primitive:
        case TypeKind::Id:
        case TypeKind::Void:
        case TypeKind::Error:
        {
             return type;
        }
           
    }

    return type;
}

} // namespace rex