#include <lexer.h>
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string& sourceCode)
    : source(sourceCode), position(0), line(1), column(1) {}

char Lexer::currentChar() {
    return position < source.size() ? source[position] : '\0';
}

void Lexer::advance() {
    if(currentChar() == '\n') {
        line ++;
        column = 1;
    } else {
        column ++;
    }
    position ++;
}

void Lexer::skipWhitespace() {
    while (std::isspace(currentChar())) {
        advance();
    }
}

Token Lexer::tokenizeKeywordOrIdentifier() {
    size_t start = position;
    while (std::isalnum(currentChar()) || currentChar() == '_') {
        advance();
    }
    std::string value = source.substr(start, position - start);

    //simple keyword check:
    if (value == "module" || value == "endmodule" || value == "input" || value == "output") {
        return {TokenType::Keyword, value, line, column};
    }
    return {TokenType::Identifier, value, line, column};
}

Token Lexer::tokenizeNumber() {
    size_t start = position;
    while (std::isdigit(currentChar())) {
        advance();
    }
    std::string value = source.substr(start, position - start);
    return {TokenType::Number, value, line, column};
}

Token Lexer::tokenizeSymbol() {
    char symbol = currentChar();
    advance();
    return {TokenType::Symbol, std::string(1, symbol), line, column};
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (currentChar() != '\0') {
        skipWhitespace();
        if (std::isalpha(currentChar()) || currentChar() == '_') {
            tokens.push_back(tokenizeKeywordOrIdentifier());
        } else if (std::isdigit(currentChar())) {
            tokens.push_back(tokenizeNumber());
        } else if (std::ispunct(currentChar())) {
            tokens.push_back(tokenizeSymbol());
        } else {
            advance(); //skip unknown characters
        }
    }
    return tokens;
}








