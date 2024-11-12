#pragma once

#include <string>
#include <unordered_map>
#include "../Utils/TokenUtils.h"

enum TokenType {
    TOKEN_TYPE_MESSAGE,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_OPENING_CURLY,
    TOKEN_TYPE_CLOSING_CURLY,
    TOKEN_TYPE_EQUALS,
    TOKEN_TYPE_INDEX,
    TOKEN_TYPE_SEMI_COLON,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_INT32,
    TOKEN_TYPE_UINT32,
    TOKEN_TYPE_DOUBLE,
    TOKEN_TYPE_BYTES,
    TOKEN_TYPE_BOOL,
    TOKEN_TYPE_INT64,
    TOKEN_TYPE_UINT64,
    TOKEN_TYPE_LIST
};

class Token {
    public:
        Token(std::string value, int line);
        ~Token();
        TokenType getType();
        std::string getValue();
        int getLine();
    private:
        TokenType type;
        std::string value;
        int line;
        static std::unordered_map<std::string, TokenType> tokenTypeMap;
};