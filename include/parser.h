#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <memory>
#include <vector>
#include <string>

/*
 Header file for parser
 Parser will analyse the sequence of tokens from the lexer and will check Verilog's syntax and rules
 Creates Abstract Syntax Tree (AST) for further processing
*/

enum class ASTNodeType {
    Module,
    Input,
    Output,
    Assign,
    Identifier,
    Operation
};

struct ASTNode {
    ASTNodeType type;
    std::string value;
    std::vector<std::shared_ptr<ASTNode>> children;

    ASTNode(ASTNodeType type, const std::string& value)
        : type(type), value(value) {}
};

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens);
    std::shared_ptr<ASTNode> parse();

private:
    std::vector<Token> tokens;
    size_t currentIndex;

    Token currentToken();
    void advance();

    std::shared_ptr<ASTNode> parseModule();
    std::shared_ptr<ASTNode> parsePortList();
    std::shared_ptr<ASTNode> parseAssign();
    std::shared_ptr<ASTNode> parseIdentifier();
};

#endif // PARSE_H
