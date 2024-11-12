#include "Token.h"

std::unordered_map<std::string, TokenType> Token::tokenTypeMap = {
    { "int32", TOKEN_TYPE_INT32 },
    { "uint32", TOKEN_TYPE_UINT32 },
    { "double", TOKEN_TYPE_DOUBLE },
    { "bytes", TOKEN_TYPE_BYTES },
    { "message", TOKEN_TYPE_MESSAGE },
    { "string", TOKEN_TYPE_STRING },
    { "}", TOKEN_TYPE_CLOSING_CURLY },
    { "{", TOKEN_TYPE_OPENING_CURLY },
    { "=", TOKEN_TYPE_EQUALS },
    { ";", TOKEN_TYPE_SEMI_COLON },
    { "bool", TOKEN_TYPE_BOOL },
    { "uint64", TOKEN_TYPE_UINT64 },
    { "int64", TOKEN_TYPE_INT64 },
    { "list", TOKEN_TYPE_LIST}
};

Token::Token(std::string value, int line) {
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

std::string Token::getValue() {
    return value;
}

int Token::getLine() {
    return line;
}