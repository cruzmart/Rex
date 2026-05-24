#include <iostream>
#include <memory>

#include "RexLexer.h"
#include "RexParser.h"

#include "passes/rex_alias_pass.h"
#include "passes/rex_assignment_pass.h"
#include "passes/rex_break_pass.h"
#include "passes/rex_exp_pass.h"
#include "passes/rex_return_pass.h"
#include "passes/rex_toplevel_order_pass.h"
#include "rex_ast_build.h"
#include "rex_ast_nodes.h"


#include "backend/rex_backend.h"



using namespace antlr4;
using namespace rex;

int main(int argc, char* argv[]) {

    std::string filename = argv[1];
    std::string fileDirectory = "tests/" + filename + ".txt";

    std::ifstream file(fileDirectory);
    if (!file) {
        std::cerr << "Failed to open file: " << fileDirectory << "\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();


    ANTLRInputStream input(buffer.str());
    RexLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    RexParser parser(&tokens);

    RexParser::FileContext* parse_tree = parser.file();

    RexAstBuild builder;
    auto ast_any = builder.visit(parse_tree);
    auto ast = std::any_cast<std::shared_ptr<FileAst>>(ast_any);

    bool debug = false;

    /////// Aliasing Pass /////
    auto global_scope = std::make_shared<Scope>();
    AliasPass pass_alias(global_scope, debug);
    pass_alias.visit(ast);

    ////// Expr Pass //////
    auto global_scope_2 = std::make_shared<Scope>();
    ExprPass pass_expr(global_scope_2);
    pass_expr.visit(ast);

    ///// Assign Pass /////
    AssignmentCheckPass pass_assign;
    pass_assign.visit(ast);

    //// Return Pass /////
    ReturnCheckPass pass_return;
    pass_return.visit(ast);

    //// Break Pass /////
    BreakCheckPass pass_break;
    pass_break.visit(ast);

    //// Function Hoisting Pass /////
    TopLevelOrderPass pass_funcs_hoisting;
    pass_funcs_hoisting.visit(ast);

    ast->dump(std::cout, 0);

    BackEnd backend;
    backend.emitMain(ast);

    std::ofstream ofs("output.ll");
    backend.dumpLLVM(ofs, true);

    return 0;
}