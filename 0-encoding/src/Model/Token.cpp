#include "Token.h"

unordered_map<string, TokenType> Token::tokenTypeMap = {
    { "int32", TOKEN_TYPE_INT32 },
    { "message", TOKEN_TYPE_MESSAGE },
    { "string", TOKEN_TYPE_STRING },
    { "}", TOKEN_TYPE_CLOSING_CURLY },
    { "{", TOKEN_TYPE_OPENING_CURLY },
    { "=", TOKEN_TYPE_EQUALS },
    { ";", TOKEN_TYPE_SEMI_COLON }
};

Token::Token(string value, int line) {
    this->value = value;
    this->line = line;

    try {
        this->type = Token::tokenTypeMap.at(value);
    } catch (...) {
        if (TokenUtils::isDigit(value.at(0))) {
            this->type = TOKEN_TYPE_INDEX;
        }
        else {
            this->type = TOKEN_TYPE_IDENTIFIER;
        }
    }
}

Token::~Token() {

}

TokenType Token::getType() {
    return type;
}

string Token::getValue() {
    return value;
}

int Token::getLine() {
    return line;
}