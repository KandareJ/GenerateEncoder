#include "tokenizer.h"

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

vector<string> Tokenizer::tokenize() {
    state = 0;
    char current;
    index = 0;

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
    if (current == ' ' || current == '\n') {
        tokens.push_back(currentToken);
        currentToken = "";
        state = 0;
    }
    else if (current == '{') {
        tokens.push_back(currentToken);
        currentToken = "";
        tokens.push_back("{");
        state = 0;
    }
    else if (current == '}') {
        tokens.push_back(currentToken);
        currentToken = "";
        tokens.push_back("}");
        state = 0;
    }
    else if (current == ';') {
        tokens.push_back(currentToken);
        currentToken = "";
        tokens.push_back(";");
        state = 0;
    }
    else if (isDigit(current)) {
        currentToken += current;
    }
    else if (current == '=') {
        tokens.push_back(currentToken);
        currentToken = "";
        tokens.push_back("=");
        state = 0;
    }
    else {
        throw index;
    }
}

void Tokenizer::state1(char current) {
    if (current == ' ' || current == '\n') {
        tokens.push_back(currentToken);
        currentToken = "";
        state = 0;
    }
    else if (current == '{') {
        tokens.push_back(currentToken);
        currentToken = "";
        tokens.push_back("{");
        state = 0;
    }
    else if (current == '}') {
        tokens.push_back(currentToken);
        currentToken = "";
        tokens.push_back("}");
        state = 0;
    }
    else if (isDigit(current)) {
        currentToken += current;
    }
    else if (isCharacter(current)) {
        currentToken += current;
    }
    else if (current == '=') {
        tokens.push_back(currentToken);
        currentToken = "";
        tokens.push_back("=");
        state = 0;
    }
    else if (current == ';') {
        tokens.push_back(currentToken);
        currentToken = "";
        tokens.push_back(";");
        state = 0;
    }
    else {
        throw index;
    }
}

void Tokenizer::state0(char current) {
    if (current == ' ' || current == '\n') {
        // whitespace, ignore
    }
    else if (current == '{') {
        tokens.push_back("{");
    }
    else if (current == '}') {
        tokens.push_back("}");
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
        tokens.push_back("=");
    }
    else if (current == ';') {
        tokens.push_back(";");
    }
    else {
        throw index;
    }
}



