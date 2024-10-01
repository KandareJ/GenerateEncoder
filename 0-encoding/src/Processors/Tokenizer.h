#pragma once

#include <vector>
#include <string.h>
#include <istream>

#include "../Model/Token.h"
#include "../Exceptions/SyntaxError.h"

using namespace std;

class Tokenizer {
    public:
        Tokenizer(istream* input);
        ~Tokenizer();
        vector<Token> tokenize();
    private:
        istream* input;
        vector<Token> tokens;
        string currentToken;
        int state;
        int index;
        int line;
        void state0(char current);
        void state1(char current);
        void state2(char current);
};