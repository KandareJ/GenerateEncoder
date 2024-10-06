#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "./Processors/Tokenizer.h"
#include "./Model/Token.h"
#include "./Exceptions/SyntaxError.h"

using namespace std;

int main() {
    ifstream* file = new ifstream();
    file->open("example.msg");
    Tokenizer tokenizer = Tokenizer(file);

    try {
        vector<Token> tokens = tokenizer.tokenize();
        for (int i = 0; i < tokens.size(); i++) {
            cout << tokens.at(i).getValue() << endl;
        }

    } catch (SyntaxError syntaxError) {
        cout << syntaxError.getMessage() << endl;
    }

    return 0;
}