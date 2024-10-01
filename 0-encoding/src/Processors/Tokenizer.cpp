#include "Tokenizer.h"

bool isDigit(char character) {
    return character >= '0' && character <= '9';
}

bool isCharacter(char character) {
    return (character >= 'A' && character <= 'Z')
        || (character >= 'a' && character <= 'z');
}

Tokenizer::Tokenizer(istream* input) {
    this->input = input;
}

Tokenizer::~Tokenizer() {
}

vector<Token> Tokenizer::tokenize() {
    state = 0;
    char current;
    index = 0;
    line = 1;

    while (input->get(current)) {
        index++;
        if (state == 0) {
            state0(current);
        }
        else if (state == 1) {
            state1(current);
        }
        else {
            state2(current);
        }
    }

    return tokens;
}

void Tokenizer::state2(char current) {
    if (current == ' ') {
        tokens.push_back(Token(currentToken, INDEX, line));
        currentToken = "";
        state = 0;
    }
    else if (current == '\n') {
        tokens.push_back(Token(currentToken, INDEX, line));
        currentToken = "";
        state = 0;
        line++;
    }
    else if (current == '{') {
        tokens.push_back(Token(currentToken, INDEX, line));
        currentToken = "";
        tokens.push_back(Token("{", OPENING_CURLY, line));
        state = 0;
    }
    else if (current == '}') {
        tokens.push_back(Token(currentToken, INDEX, line));
        currentToken = "";
        tokens.push_back(Token("}", CLOSING_CURLY, line));
        state = 0;
    }
    else if (current == ';') {
        tokens.push_back(Token(currentToken, INDEX, line));
        currentToken = "";
        tokens.push_back(Token(";", SEMI_COLON, line));
        state = 0;
    }
    else if (isDigit(current)) {
        currentToken += current;
    }
    else if (current == '=') {
        tokens.push_back(Token(currentToken, INDEX, line));
        currentToken = "";
        tokens.push_back(Token("=", EQUALS, line));
        state = 0;
    }
    else {
        throw SyntaxError(line, current);
    }
}

void Tokenizer::state1(char current) {
    if (current == ' ' || current == '\n') {
        tokens.push_back(Token(currentToken, line));
        currentToken = "";
        state = 0;
    }
    
    else if (current == '\n') {
        tokens.push_back(Token(currentToken, line));
        currentToken = "";
        state = 0;
        line++;
    }
    else if (current == '{') {
        tokens.push_back(Token(currentToken, line));
        currentToken = "";
        tokens.push_back(Token("{", OPENING_CURLY, line));
        state = 0;
    }
    else if (current == '}') {
        tokens.push_back(Token(currentToken, line));
        currentToken = "";
        tokens.push_back(Token("}", CLOSING_CURLY, line));
        state = 0;
    }
    else if (current == ';') {
        tokens.push_back(Token(currentToken, line));
        currentToken = "";
        tokens.push_back(Token(";", SEMI_COLON, line));
        state = 0;
    }
    else if (isDigit(current)) {
        currentToken += current;
    }
    else if (isCharacter(current)) {
        currentToken += current;
    }
    else if (current == '=') {
        tokens.push_back(Token(currentToken, line));
        currentToken = "";
        tokens.push_back(Token("=", EQUALS, line));
        state = 0;
    }
    else {
        throw SyntaxError(line, current);
    }
}

void Tokenizer::state0(char current) {
    if (current == ' ') {
        // whitespace, ignore
    }
    else if (current == '\n') {
        line++;
    }
    else if (current == '{') {
        tokens.push_back(Token("{", OPENING_CURLY, line));
    }
    else if (current == '}') {
        tokens.push_back(Token("}", CLOSING_CURLY, line));
    }
    else if (isDigit(current)) {
        currentToken += current;
        state = 2;
    }
    else if (isCharacter(current)) {
        currentToken += current;
        state = 1;
    }
    else if (current == '=') {
        tokens.push_back(Token("=", EQUALS, line));
    }
    else if (current == ';') {
        tokens.push_back(Token(";", SEMI_COLON, line));
    }
    else {
        throw SyntaxError(line, current);
    }
}



