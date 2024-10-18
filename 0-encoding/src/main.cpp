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
#include "./Processors/Temp.h"
#include "./Utils/StringUtils.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Error: expected 3 arguments, got " << argc - 1 << endl;
        cout << "Usage: EncodeGen [json | jbuf] <input file> <output directory>" << endl;
        return -1;
    }

    string type = string(argv[1]);
    string inputPath = string(argv[2]);
    string outputPath = string(argv[3]);

    ifstream* file = new ifstream();
    file->open(inputPath);
    Tokenizer tokenizer = Tokenizer(file);
    Parser parser = Parser();
    Temp temp = Temp();

    try {
        unordered_map<string, Message> messages = parser.parse(tokenizer.tokenize());
        unordered_map<string, Message>::iterator it;

        ofstream utilHeaderFile;
        utilHeaderFile.open(outputPath + "/JsonUtils.h");
        utilHeaderFile << temp.generateUtilHeader();
        utilHeaderFile.close();

        ofstream utilCppFile;
        utilCppFile.open(outputPath + "/JsonUtils.cpp");
        utilCppFile << temp.generateUtilCpp();
        utilCppFile.close();

        for (it = messages.begin(); it != messages.end(); it++) {
            string filepath = outputPath + "/" + StringUtils::capitalize(it->first);
            ofstream headerFile;
            ofstream cppFile;

            headerFile.open(filepath + ".h");
            cppFile.open(filepath + ".cpp");

            headerFile << temp.generateHeader(it->second);
            cppFile << temp.generateCpp(it->second);

            headerFile.close();
            cppFile.close();
        }

    } catch (Error error) {
        cout << error.getMessage() << endl;
    }

    return 0;
}