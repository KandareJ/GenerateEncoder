#pragma once

#include <string>
#include <unordered_map>
#include "../Utils/TokenUtils.h"

using namespace std;

enum TokenType {
    TOKEN_TYPE_MESSAGE,
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_OPENING_CURLY,
    TOKEN_TYPE_CLOSING_CURLY,
    TOKEN_TYPE_EQUALS,
    TOKEN_TYPE_INDEX,
    TOKEN_TYPE_SEMI_COLON,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_INT32
};

class Token {
    public:
        Token(string value, int line);
        ~Token();
        TokenType getType();
        string getValue();
        int getLine();
    private:
        TokenType type;
        string value;
        int line;
        static unordered_map<string, TokenType> tokenTypeMap;
};