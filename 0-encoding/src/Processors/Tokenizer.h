#pragma once

#include <vector>
#include <string.h>
#include <istream>

#include "../Model/Token.h"
#include "../Exceptions/SyntaxError.h"
#include "../Utils/TokenUtils.h"

using namespace std;

class TokenizerState;

class Tokenizer {
    public:
        Tokenizer(istream* input);
        ~Tokenizer();
        vector<Token> tokenize();
    private:
        friend class TokenizerState;
        void changeState(TokenizerState* newState);
        TokenizerState* state;
        istream* input;
        vector<Token> tokens;
        string currentToken;
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