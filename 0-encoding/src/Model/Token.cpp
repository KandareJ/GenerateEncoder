#include "Token.h"

Token::Token(string value, TokenType type, int line) {
    this->value = value;
    this->type = type;
    this->line = line;
}

// use a map to optimize
Token::Token(string value, int line) {
    this->value = value;
    this->line = line;

    if (value == "int32") {
        this->type = INT32;
    }
    else if (value == "message") {
        this->type = MESSAGE;
    }
    else if (value == "string") {
        this->type = STRING;
    }
    else if (value == "}") {
        this->type = CLOSING_CURLY;
    }
    else if (value == "{") {
        this->type = OPENING_CURLY;
    }
    else if (value == "=") {
        this->type = EQUALS;
    }
    else if (value == ";") {
        this->type = SEMI_COLON;
    }
    else {
        this->type = IDENTIFIER;
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