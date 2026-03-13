#include "rex_type_checker.h"
#include "rex_ast_nodes.h"
#include "rex_scope.h"
#include "rex_symbol.h"
#include "rex_types.h"
#include <algorithm>
#include <memory>

using namespace rex;


// ---------------------------------------------------------
// TypeChecker ctor
// ---------------------------------------------------------
TypeChecker::TypeChecker(std::shared_ptr<Scope> g)
    : global(std::move(g)) 
{
    current_scope = global;
}

// ---------------------------------------------------------
// Error helper
// ---------------------------------------------------------
void TypeChecker::error(const std::string msg, const size_t line) {
    std::cerr << "[type error] line " << line << ": " << msg << "\n";
    exit(1);
}

// =========================================================
// TOP-LEVEL FILE CHECK
// =========================================================
void TypeChecker::check(std::shared_ptr<FileAst> file) {
    for (auto& item : file->items) {
        if (auto stmt = std::dynamic_pointer_cast<Stmt>(item))
            check_stmt(stmt);
        else if (auto fn = std::dynamic_pointer_cast<FunctionDecl>(item))
            check_function(fn);
        else if (auto td = std::dynamic_pointer_cast<TypeDecl>(item))
            check_type_decl(td);
        else
            error("Unknown top-level AST node", item->loc.line);
    }
}

// =========================================================
// EXPRESSION TYPE CHECK
// =========================================================
type_ptr TypeChecker::check_expr(std::shared_ptr<Expr> e) {

    if (auto lit = std::dynamic_pointer_cast<LiteralExpr>(e))
        return check_literal(lit);

    if (auto id = std::dynamic_pointer_cast<IdExpr>(e))
        return check_id(id);

    if (auto bin = std::dynamic_pointer_cast<BinaryExpr>(e))
        return check_binary(bin);

    if (auto un = std::dynamic_pointer_cast<UnaryExpr>(e))
        return check_unary(un);

    if (auto call = std::dynamic_pointer_cast<CallExpr>(e))
        return check_call(call);

    if (auto idx = std::dynamic_pointer_cast<IndexExpr>(e))
        return check_index(idx);

    if (auto tup = std::dynamic_pointer_cast<TupleExpr>(e))
        return check_tuple(tup);

    if (auto arr = std::dynamic_pointer_cast<ArrayExpr>(e))
        return check_array(arr);

    error("Unknown expression", e->loc.line);
    return std::make_shared<Type>(TypeKind::Error);
}


// ---------------------------------------------------------
// Literal
// ---------------------------------------------------------
type_ptr TypeChecker::check_literal(std::shared_ptr<LiteralExpr> e) {
    // The parser already set PrimType on literal nodes.
    return e->type;
}

// ---------------------------------------------------------
// Tuple
// ---------------------------------------------------------
type_ptr TypeChecker::check_tuple(std::shared_ptr<TupleExpr> t) {

    auto tupleT = std::make_shared<TupleType>();

    for (auto& elem : t->elements) {
        auto typeE = check_expr(elem);
        tupleT->tuple_types.push_back(typeE);
    }

    t->type = tupleT;
    return tupleT;
}


type_ptr TypeChecker::check_id(std::shared_ptr<IdExpr> id){
    std::shared_ptr<Symbol> sym = current_scope->resolve(id->name);

    if (!sym)
        error("Undefined identifier '" + id->name + "'", id->loc.line);

    id->resolved = sym;
    id->type = sym->type;

    return id->type;


}

// ---------------------------------------------------------
// Array literal
// ---------------------------------------------------------
type_ptr TypeChecker::check_array(std::shared_ptr<ArrayExpr> a) {

    if (a->elements.empty()) {
        error("Empty array literal is not allowed", a->loc.line);
    }

    // All elements must have the same type
    auto firstT = check_expr(a->elements[0]);

    if(firstT->fundamental_kind != rex::TypeKind::Primitive){
        error("Array must contain primitive type values", a->loc.line);
    }

    auto first_t_prim = std::dynamic_pointer_cast<PrimType>(firstT);

    for (auto& e : a->elements) {
        auto t2 = check_expr(e);

        if(t2->fundamental_kind != rex::TypeKind::Primitive){
            error("Array must contain primitive type values", a->loc.line);
        }

        auto t2_prim = std::dynamic_pointer_cast<PrimType>(t2);
        
        if(first_t_prim->prim_type != t2_prim->prim_type){
            error("Array values are not of the same type", a->loc.line);
        }
    }

    auto arrT = std::make_shared<ArrayType>(firstT, (int)a->elements.size());
    a->type = arrT;

    return arrT;
}


type_ptr TypeChecker::check_binary(std::shared_ptr<BinaryExpr> b){

    auto lt = check_expr(b->lhs);
    auto rt = check_expr(b->rhs);

    // TODO: implement real operator rules
    // check what the operator is and depending on it some values cannot be done

    // auto op = b->operation;

    // switch(op){
    //     case rex::BinaryOp::ADD:
    //     case rex::BinaryOp::SUB:
    //     case rex::BinaryOp::MUL:
    //     case rex::BinaryOp::DIV:
    //     case rex::BinaryOp::MOD:

    //     case rex::BinaryOp::EQ:
    //     case rex::BinaryOp::NEQ:
    //     case rex::BinaryOp::LT:
    //     case rex::BinaryOp::GT:
    //     case rex::BinaryOp::LE:
    //     case rex::BinaryOp::GE:

    //     case rex::BinaryOp::AND:
    //     case rex::BinaryOp::OR:

    //     case rex::BinaryOp::PIPE:
    //     case rex::BinaryOp::RANGE:
    // }


    // For now, enforce lt == rt
    if (lt->fundamental_kind != rt->fundamental_kind)
        error("Binary operands must have same type", b->loc.line);

    b->type = lt;
    return lt;

    
}

// ---------------------------------------------------------
// Unary operations
// ---------------------------------------------------------
type_ptr TypeChecker::check_unary(std::shared_ptr<UnaryExpr> u) {

    auto t = check_expr(u->rhs);

    // TODO: operator rules
    u->type = t;
    return t;
}

type_ptr TypeChecker::check_call(std::shared_ptr<CallExpr> c){return nullptr;}
type_ptr TypeChecker::check_index(std::shared_ptr<IndexExpr> i){return nullptr;}
type_ptr TypeChecker::check_function(std::shared_ptr<FunctionDecl> f){return nullptr;}


type_ptr TypeChecker::resolve_id(const std::string name){return nullptr;}

// ---- Statements ----
void TypeChecker::check_stmt(std::shared_ptr<Stmt> s){
    // check if it is a 'let'
    if (auto stmt = std::dynamic_pointer_cast<LetStmt>(s)){
        check_let(stmt);
    }

    // check if is 'if'
    // check if it is 'while'
    // check if it is a 'type decl'
    // check if it is a 'for'
    // check if it is a 'return'
    // check if it is a 'assign'
}
void TypeChecker::check_let(std::shared_ptr<LetStmt> s){
    // we are going to make the symbol AFTER we are done checking the type of the decleration and the initialized statement are the same
    // example let a : Int = 2;
}
void TypeChecker::check_assign(std::shared_ptr<AssignStmt> s){}
void TypeChecker::check_return(std::shared_ptr<ReturnStmt> s){}
void TypeChecker::check_if(std::shared_ptr<IfStmt> s){}
void TypeChecker::check_while(std::shared_ptr<WhileStmt> s){}
void TypeChecker::check_loop(std::shared_ptr<LoopStmt> s){}
void TypeChecker::check_for(std::shared_ptr<ForStmt> s){}
void TypeChecker::check_type_decl(std::shared_ptr<TypeDecl> t){}

// ---------------------------------------------------------
// Enter new scope
// ---------------------------------------------------------
void TypeChecker::push_scope() {
    current_scope = current_scope->push();
}

// ---------------------------------------------------------
// Leave scope
// ---------------------------------------------------------
void TypeChecker::pop_scope() {
    current_scope = current_scope->parent;
}