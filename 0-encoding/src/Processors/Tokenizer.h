#pragma once

#include <vector>
#include <string>

#include "../Model/Token.h"
#include "../Exceptions/SyntaxError.h"
#include "../Utils/TokenUtils.h"

class TokenizerState;

class Tokenizer {
    public:
        Tokenizer();
        ~Tokenizer();
        std::vector<Token> tokenize(std::string input);
    private:
        friend class TokenizerState;
        void reset();
        void changeState(TokenizerState* newState);
        TokenizerState* state;
        std::vector<Token> tokens;
        std::string currentToken;
        int line;
};

class TokenizerState {
    public:
        virtual void readCharacter(char character) = 0;
    protected:
        void changeState(TokenizerState* newState);
        void appendToToken(char current);
        void pushToken();
        void appendAndPushToken(char current);
        void throwError(char current);
        void incrementLine();
        Tokenizer* tokenizer;
};

class BaseState : public TokenizerState {
    public:
        BaseState(Tokenizer* tokenizer);
        virtual void readCharacter(char character);
};

class VariableState : public TokenizerState {
    public:
        VariableState(Tokenizer* tokenizer);
        virtual void readCharacter(char character);
};

class IndexState : public TokenizerState {
    public:
        IndexState(Tokenizer* tokenizer);
        virtual void readCharacter(char character);
};