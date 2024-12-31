#include "parser.h"
#include <stdexcept>
#include <iostream>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), currentIndex(0) {}

Token Parser::currentToken() {
    return currentIndex < tokens.size() ? tokens[currentIndex] : Token{TokenType::Unknown, "", -1, -1};
}

void Parser::advance() {
    if (currentIndex < tokens.size()) {
        currentIndex++;
    }
}

std::shared_ptr<ASTNode> Parser::parse() {
    return parseModule();
}

std::shared_ptr<ASTNode> Parser::parseModule() {

    //check 'module' keyword
    if (currentToken().type != TokenType::Keyword || currentToken().value != "module") {
        throw std::runtime_error("Expected 'module' keyword");
    }
    advance(); //Consume 'module'

    if (currentToken().type != TokenType::Identifier) {
        throw std::runtime_error("Expected module name");
    }

    auto moduleNode = std::make_shared<ASTNode>(ASTNodeType::Module, currentToken().value);
    advance(); //Consume module name

    if (currentToken().value != "(") {
        throw std::runtime_error("Expected '(' after module name");
    }
    advance(); //consume '('
    
    moduleNode->children.push_back(parsePortList());

    if (currentToken().value != ";") {
        std::cerr << "Error: Expected ';' after port list at line " << currentToken().line
                  << ", column " << currentToken().column << "\n";
        throw std::runtime_error("Module parsing error");
    }
    advance(); //consume ';'

    while (currentToken().value != "endmodule" && currentToken().type != TokenType::Unknown) {
        if (currentToken().type == TokenType::Keyword && currentToken().value == "assign") {
            moduleNode->children.push_back(parseAssign());
        } else {
            advance(); //skip unrecognized tokens for now
        }
    }

    if (currentToken().value != "endmodule") {
        throw std::runtime_error("Expected 'endmodule'");
    }
    advance(); //Consume 'endmodule'

    return moduleNode;
}

std::shared_ptr<ASTNode> Parser::parsePortList() {
    auto portListNode = std::make_shared<ASTNode> (ASTNodeType::Identifier, "ports");

    while (currentToken().value != ")" && currentToken().type != TokenType::Unknown) {
        if (currentToken().type == TokenType::Keyword &&
            (currentToken().value == "input" || currentToken().value == "output")) {

            auto portNode = std::make_shared<ASTNode>(
                    currentToken().value == "input" ? ASTNodeType::Input : ASTNodeType::Output,
                    currentToken().value);
            advance(); //consume 'input' or 'output'
                
            while (currentToken().type == TokenType::Identifier) {
                portNode->children.push_back(parseIdentifier());

            if (currentToken().value == ",") {
                advance(); //skip ','
            } else {
                    break; //No more ports of this type
                }
            }
        portListNode->children.push_back(portNode);
    } else if (currentToken().value == ",") {
        advance();
    } else {
        std::cerr << "Error: Unexpected token '" << currentToken().value << "' in port list at line "
                  << currentToken().line << ", column " << currentToken().column << "\n";
        throw std::runtime_error("Port list parsing error");
        }
    }
    if (currentToken().value == ")") {
        advance(); //consume ')'
    } else {
        std::cerr << "Error: Port list not properly closed with ')' at line "
                  << currentToken().line << ", column " << currentToken().column << "\n";
        throw std::runtime_error("Port list parsing error");
    }
    return portListNode;
}

std::shared_ptr<ASTNode> Parser::parseAssign() {
    advance(); //consume 'assign'
    auto assignNode = std::make_shared<ASTNode>(ASTNodeType::Assign, "assign");

    if (currentToken().type == TokenType::Identifier) {
         assignNode->children.push_back(parseIdentifier());
    }

    if (currentToken().value != "=") {
        throw std::runtime_error("Expected '=' in assignment");
    }
    advance(); //consume '='

    if (currentToken().type == TokenType::Identifier) {
        assignNode->children.push_back(parseIdentifier());
    }

    return assignNode;
}

std::shared_ptr<ASTNode> Parser::parseIdentifier() {
    if (currentToken().type != TokenType::Identifier) {
        throw std::runtime_error("Expected Identifier");
    }
    auto identifierNode = std::make_shared<ASTNode>(ASTNodeType::Identifier, currentToken().value);
    advance(); //consume identifier
    return identifierNode;
}
