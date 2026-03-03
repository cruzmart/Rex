#include <iostream>
#include <memory>

#include "RexLexer.h"
#include "RexParser.h"

#include "rex_ast_build.h"
#include "rex_ast_nodes.h"
#include "rex_type_checker.h"

using namespace antlr4;
using namespace rex;

int main() {
    std::ifstream file("tests/the_second.txt");
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

    ast->dump(std::cout, 0);

    // 

    // Type Checking Pass
    // auto global_scope = std::make_shared<Scope>();
    // TypeChecker type_pass(global_scope);
    // type_pass.check(ast);


    return 0;
}