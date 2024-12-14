#include "Processors/Program.h"
#include <iostream>

using namespace std;

int Program::run(int argc, char** argv) {
    try {
        Command command = parseCommand(argc, argv);
        Tokenizer tokenizer;
        Parser parser;
        CodeGenerator* codeGenerator = new JsonGenerator();
        // TODO: initialize codeGenerator based on command args

        std::string input = FileUtil::readFile(command.getInputFile());
        cout << input << endl;
        std::vector<Token> tokens = tokenizer.tokenize(input);
        std::unordered_map<std::string, Message> messages = parser.parse(tokens);
        std::vector<OutputFile> outputs = codeGenerator->generateCode(messages);

        for (int i = 0; i < outputs.size(); i++) {
            std::string path = StringUtils::createPath(command.getOutputDirectory(), outputs.at(i).getPath());
            FileUtil::writeFile(path, outputs.at(i).getContents());
        }

        delete codeGenerator;
        return 0;
    } catch (Error e) {
        std::cout << e.getMessage() << std::endl;
        return -1;
    }
}

Command Program::parseCommand(int argc, char** argv) {
    CliParser cli;
    return cli.parseCommand(argc, argv);
}

