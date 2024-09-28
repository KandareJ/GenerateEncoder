#pragma once

#include <vector>
#include <string.h>
#include <istream>

using namespace std;

class Tokenizer {
    public:
        Tokenizer(istream* input);
        ~Tokenizer();
        vector<string> tokenize();
    private:
        istream* input;
        vector<string> tokens;
        string currentToken;
        int state;
        int index;
        void state0(char current);
        void state1(char current);
        void state2(char current);
};