#include "Processors/Program.h"

int Program::run(int argc, char** argv) {
    try {
        Command command = parseCommand(argc, argv);
        Tokenizer tokenizer;
        Parser parser;
        CodeGenerator* codeGenerator = initializeCodeGenerator(command);

        std::string input = FileUtil::readFile(command.getInputFile());
        std::vector<Token> tokens = tokenizer.tokenize(input);
        std::unordered_map<std::string, Message> messages = parser.parse(tokens);
        std::vector<OutputFile> outputs = codeGenerator->generateCode(messages);

        for (unsigned int i = 0; i < outputs.size(); i++) {
            std::string path = StringUtils::createPath(command.getOutputDirectory(), outputs.at(i).getPath());
            FileUtil::writeFile(path, outputs.at(i).getContents());
            std::cout << "Built file " << outputs.at(i).getPath() << " at location " << path << std::endl;
        }

        delete codeGenerator;
        return 0;
    } catch (Error e) {
        std::cout << e.getMessage() << std::endl;
        return -1;
    }
}

CodeGenerator* Program::initializeCodeGenerator(Command command) {
    if (command.getOutputLanguage() == "cpp") {
        if (command.getEncodingType() == "jbuf") {
            return new JbufGenerator();
        }
        else {
            return new JsonGenerator();
        }
    }
    else {
        return new JsonGenerator();
    }
}

Command Program::parseCommand(int argc, char** argv) {
    CliParser cli;
    return cli.parseCommand(argc, argv);
}

