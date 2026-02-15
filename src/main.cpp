#include <iostream>
#include <memory>

#include "RexLexer.h"
#include "RexParser.h"

#include "rex_ast_build.h"
#include "rex_ast_nodes.h"
#include "rex_resolver.h"   // <-- ADD THIS

using namespace antlr4;
using namespace rex;

int main() {
    std::ifstream file("tests/the_sixth.txt");
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
    auto ast = std::any_cast<std::shared_ptr<file_ast>>(ast_any);

    // ---------------------------------------------------
    // RUN THE RESOLVER
    // ---------------------------------------------------
    resolver R;
    R.debug = false;   // Optional: show scope + lookup logs

    std::cout << ">>> RESOLVE\n";
    R.resolve(*ast);
    std::cout << ">>> DONE\n";

    return 0;
}