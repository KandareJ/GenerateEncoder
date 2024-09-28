#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "tokenizer.h"

using namespace std;

int main() {
    ifstream* file = new ifstream();
    file->open("example.msg");
    Tokenizer tokenizer = Tokenizer(file);

    try {
        vector<string> tokens = tokenizer.tokenize();

        for (int i = 0; i < tokens.size(); i++) {
            cout << tokens.at(i) << endl;
        }

    } catch (int index) {
        cout << "You errored out " << index << endl;
    }

    return 0;
}