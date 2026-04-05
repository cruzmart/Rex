#include <iostream>
#include <memory>

#include "RexLexer.h"
#include "RexParser.h"

#include "passes/rex_alias_pass.h"
#include "passes/rex_assignment_pass.h"
#include "passes/rex_break_pass.h"
#include "passes/rex_exp_pass.h"
#include "passes/rex_return_pass.h"
#include "rex_ast_build.h"
#include "rex_ast_nodes.h"


#include "backend/rex_backend.h"



using namespace antlr4;
using namespace rex;

int main() {
    std::ifstream file("tests/the_zero.txt");
    if (!file) {
        std::cerr << "Failed to open .txt file\n";
        return 1;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();

    ANTLRInputStream input(buffer.str());
    RexLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    RexParser parser(&tokens);

    RexParser::FileContext* parse_tree = parser.file();

    rex_ast_build builder;
    auto ast_any = builder.visit(parse_tree);
    auto ast = std::any_cast<std::shared_ptr<FileAst>>(ast_any);


    //ast->dump(std::cout, 0);

    bool debug = false;

    /////// Aliasing Pass /////
    auto global_scope = std::make_shared<Scope>();
    AliasPass pass_alias(global_scope, debug);
    pass_alias.visit(ast);
    // std::cout << "Alias Check (Passed)" << std::endl;

    ////// Expr Pass //////
    auto global_scope_2 = std::make_shared<Scope>();
    ExprPass pass_expr(global_scope_2);
    pass_expr.visit(ast);
    // std::cout << "Expression Check (Passed)" << std::endl;

    ///// Assign Pass /////
    AssignmentCheckPass pass_assign;
    pass_assign.visit(ast);
    // std::cout << "Assignment Check (Passed)" << std::endl;

    //// Return Pass /////
    ReturnCheckPass pass_return;
    pass_return.visit(ast);
    // std::cout << "Return Check (Passed)" << std::endl;

    //// Break Pass /////
    BreakCheckPass pass_break;
    pass_break.visit(ast);
    // std::cout << "Break Check (Passed)" << std::endl;


    //ast->dump(std::cout, 0);


    BackEnd backend;
    backend.emitMain();
    std::ofstream ofs("output.ll");
    backend.dumpLLVM(ofs, true);



    return 0;
}