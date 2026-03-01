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
    std::cerr << "[type error] " << " at line " << line << ": "  << msg << "\n";
    exit(1);
}

void TypeChecker::check(std::shared_ptr<FileAst> file) {
    for (auto& item : file->items) {
        if (auto stmt = std::dynamic_pointer_cast<Stmt>(item)) {
            check_stmt(stmt);
        } else if (auto fn = std::dynamic_pointer_cast<FunctionDecl>(item)) {
            check_function(fn);
        } else if (auto type_decl = std::dynamic_pointer_cast<TypeDecl>(item)) {
            check_type_decl(type_decl);
        } else {
            throw std::runtime_error("Unknown top-level AST node");
        }
    }
}
void TypeChecker::visit(std::shared_ptr<FileAst> file){}

// ---- Expressions ----
type_ptr TypeChecker::check_expr(std::shared_ptr<Expr> e){
    // check if it is a id exp


    // check if it is a tuple

    std::cout << e->type->to_string() << std::endl;
    if(auto exp = std::dynamic_pointer_cast<TupleExpr>(e)){
        std::cout << "hello world\n";
        return check_tuple(exp);
    }
    // check if it is a literal exp
    if(auto exp = std::dynamic_pointer_cast<LiteralExpr>(e)){
        std::cout << "I am a cannibal\n";
        return check_literal(exp);
    }

    // check if it is a binary exp
    // check if it is a unary
    // check if it is a func call exp
    // check if it is a index exp
    // check if it is a tuple exp
    // check if it is a array exp

    std::cout << "no type";

    return nullptr;

}
type_ptr TypeChecker::check_literal(std::shared_ptr<LiteralExpr> e){
    auto literal_type = e->type;
    return literal_type;
}

type_ptr TypeChecker::check_tuple(std::shared_ptr<TupleExpr> t){
    std::cout << t->type->to_string() << std::endl;
    std::cout << "size of elements for tuples : " << std::endl;
    exit(0);
}


type_ptr TypeChecker::check_id(std::shared_ptr<IdExpr> id){return nullptr;}
type_ptr TypeChecker::check_binary(std::shared_ptr<BinaryExpr> b){return nullptr;}
type_ptr TypeChecker::check_unary(std::shared_ptr<UnaryExpr> u){return nullptr;}
type_ptr TypeChecker::check_call(std::shared_ptr<CallExpr> c){return nullptr;}
type_ptr TypeChecker::check_index(std::shared_ptr<IndexExpr> i){return nullptr;}
type_ptr TypeChecker::check_array(std::shared_ptr<ArrayExpr> a){return nullptr;}
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