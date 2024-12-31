#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"

/*
 printing AST
*/
void printAST(const std::shared_ptr<ASTNode>& node, int depth = 0) {
    for (int i = 0; i < depth; ++i) std::cout << " ";
    std::cout << node->value << " (" << static_cast<int>(node->type) << ")\n";
    for (const auto& child : node->children) {
        printAST(child, depth + 1);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << "<verilog_file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: File could not be opened" << argv[1] << std::endl;
        return 1;
    }

    std::string sourceCode((std::istreambuf_iterator<char>(file)), 
                            std::istreambuf_iterator<char>());

    /*
     Tokenize the source code
    */
    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        std::cout << "Token(" << static_cast<int>(token.type)
                  << ", \"" << token.value
                  << "\", Line: " << token.line
                  << ", column: " << token.column << ")" <<std::endl;
    }

    try {
        Parser parser(tokens);
        auto ast = parser.parse();

        std::cout << "Abstract Syntax Tree:\n";
        printAST(ast);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
