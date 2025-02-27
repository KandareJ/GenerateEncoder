#include "Processors/Tokenizer.h"

Tokenizer::Tokenizer() {
    reset();
}

Tokenizer::~Tokenizer() {
    delete this->state;
}

void Tokenizer::reset() {
    line = 1;
    tokens = std::vector<Token>();
    currentToken = "";
    this->state = new BaseState(this);
}

std::vector<Token> Tokenizer::tokenize(std::string input) {
    reset();

    for (unsigned int i = 0; i < input.length(); i++) {
        state->readCharacter(input.at(i));
    }

    return tokens;
}

void Tokenizer::changeState(TokenizerState* newState) {
    delete state;
    state = newState;
}

TokenizerState::~TokenizerState() {
    
}

void TokenizerState::changeState(TokenizerState* newState) {
    tokenizer->changeState(newState);
}

void TokenizerState::throwError(char current) {
    throw SyntaxError(tokenizer->line, current);
}

void TokenizerState::appendToToken(char current) {
    tokenizer->currentToken += current;
}

void TokenizerState::pushToken() {
    tokenizer->tokens.push_back(Token(tokenizer->currentToken, tokenizer->line));
    tokenizer->currentToken = "";
}

void TokenizerState::appendAndPushToken(char current) {
    appendToToken(current);
    pushToken();
}

void TokenizerState::incrementLine() {
    tokenizer->line++;
}

BaseState::BaseState(Tokenizer* tokenizer) {
    this->tokenizer = tokenizer;
}

void BaseState::readCharacter(char current) {
    switch (current) {
        case ' ':
            break;
        case 13:
        case '\n':
            incrementLine();
            break;
        case '{':
        case '}':
        case '=':
        case ';':
            appendAndPushToken(current);
            break;
        default:
            if (TokenUtils::isDigit(current)) {
                appendToToken(current);
                changeState(new IndexState(tokenizer));
            }
            else if (TokenUtils::isCharacter(current)) {
                appendToToken(current);
                changeState(new VariableState(tokenizer));
            }
            else {
                throwError(current);
            }
    };
}

VariableState::VariableState(Tokenizer* tokenizer) {
    this->tokenizer = tokenizer;
}

void VariableState::readCharacter(char current) {
    switch (current) {
        case '\n':
            incrementLine();
        case ' ':
            pushToken();
            changeState(new BaseState(tokenizer));
            break;
        case '{':
        case '}':
        case '=':
        case ';':
            pushToken();
            appendAndPushToken(current);
            changeState(new BaseState(tokenizer));
            break;
        default:
            if (TokenUtils::isDigit(current) || TokenUtils::isCharacter(current)) {
                appendToToken(current);
            }
            else {
                throwError(current);
            }
    };
}

IndexState::IndexState(Tokenizer* tokenizer) {
    this->tokenizer = tokenizer;
}

void IndexState::readCharacter(char current) {
    switch (current) {
        case '\n':
            incrementLine();
        case ' ':
            pushToken();
            changeState(new BaseState(tokenizer));
            break;
        case '{':
        case '}':
        case '=':
        case ';':
            pushToken();
            appendAndPushToken(current);
            changeState(new BaseState(tokenizer));
            break;
        default:
            if (TokenUtils::isDigit(current)) {
                appendToToken(current);
            }
            else {
                throwError(current);
            }
    };
}