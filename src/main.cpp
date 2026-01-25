#include <iostream>
#include <memory>

#include "RexLexer.h"
#include "RexParser.h"

#include "rex_ast_build.h"
#include "rex_ast_nodes.h"

using namespace antlr4;
using namespace rex;

int main() {
      std::ifstream file("tests/the_second.txt");
    if (!file) {
        std::cerr << "Failed to open tests/the_first.txt\n";
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
    auto ast = std::any_cast<std::shared_ptr<file_ast>>(ast_any);
    ast->dump(std::cout, 0); // Dump the AST to standard output.

  

    return 0;
}
