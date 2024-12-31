#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class TokenType {
    Keyword,
    Identifier,
    Number,
    Symbol,
    Unknown
};

//Token Structure
struct Token {
    TokenType type;
    std::string value;
    int line; //line number
    int column; //column number
};

class Lexer {
public:
    Lexer(const std::string& sourceCode);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t position;
    int line;
    int column;

    char currentChar();
    void advance();
    void skipWhitespace();
    Token tokenizeKeywordOrIdentifier();
    Token tokenizeNumber();
    Token tokenizeSymbol();
};

#endif //LEXER_H
