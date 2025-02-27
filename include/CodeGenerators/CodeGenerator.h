#pragma once

#include <vector>
#include <unordered_map>

#include "Model/OutputFile.h"
#include "Model/Message.h"

class CodeGenerator {
    public:
        virtual ~CodeGenerator() { }
        virtual std::vector<OutputFile> generateCode(std::unordered_map<std::string, Message> messages) = 0;
};