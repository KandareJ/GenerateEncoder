#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "Processors/Tokenizer.h"
#include "Processors/Parser.h"
#include "Processors/CliParser.h"
#include "Model/Token.h"
#include "Model/Message.h"
#include "Exceptions/Error.h"
#include "CodeGenerators/CodeGenerator.h"
#include "CodeGenerators/JsonGenerator.h"
#include "CodeGenerators/JbufGenerator.h"
#include "Utils/StringUtils.h"
#include "Utils/FileUtil.h"

class Program {
    public:
        int run(int argc, char** argv);
    private:
        Command parseCommand(int argc, char** argv);
        CodeGenerator* initializeCodeGenerator(Command command);
        std::string readInputToString(std::string inputPath);

};