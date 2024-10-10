#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "./Processors/Tokenizer.h"
#include "./Processors/Parser.h"
#include "./Model/Token.h"
#include "./Exceptions/SyntaxError.h"
#include "./Exceptions/ParseError.h"
#include "./Exceptions/BuilderError.h"
#include "./Model/Message.h"
#include "./Exceptions/Error.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Error: expected 2 arguments, got " << argc - 1 << endl;
        cout << "Usage: EncodeGen <input file> <output director>" << endl;
        return -1;
    }

    string inputPath = string(argv[1]);
    string outputPath = string(argv[2]);

    ifstream* file = new ifstream();
    file->open(inputPath);
    Tokenizer tokenizer = Tokenizer(file);
    Parser parser = Parser();

    try {
        unordered_map<string, Message> messages = parser.parse(tokenizer.tokenize());
        unordered_map<string, Message>::iterator it;
        for (it = messages.begin(); it != messages.end(); it++) {
            cout << it->second.toString() << endl;
        }
    } catch (Error error) {
        cout << error.getMessage() << endl;
    }

    return 0;
}