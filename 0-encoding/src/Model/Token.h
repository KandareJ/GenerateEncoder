#pragma once

#include <string>

using namespace std;

enum TokenType {
    MESSAGE,
    IDENTIFIER,
    OPENING_CURLY,
    CLOSING_CURLY,
    EQUALS,
    INDEX,
    SEMI_COLON,
    STRING,
    INT32
};

class Token {
    public:
        Token(string value, TokenType type, int line);
        Token(string value, int line);
        ~Token();
        TokenType getType();
        string getValue();
    private:
        TokenType type;
        string value;
        int line;
};