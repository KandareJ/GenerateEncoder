#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "CodeGenerators/CodeGenerator.h"
#include "Model/Message.h"
#include "Utils/StringUtils.h"

class CppCodeGenerator : public CodeGenerator {
    public:
        virtual std::vector<OutputFile> generateCode(std::unordered_map<std::string, Message> messages);
    protected:
        std::string generateHeader(Message message);
        virtual std::string generateAdditionalPublic(Message message);
        virtual std::string generateAdditionalPrivate(Message message);
        std::string generateCpp(Message message);
        virtual std::string generateUtilHeader() = 0;
        virtual std::string generateUtilCpp() = 0;
        virtual std::string getIncludeHeaders() = 0;
        virtual std::string generateEncode(Message message) = 0;
        virtual std::string generateDecode(Message message) = 0;
        std::string generateClassHeader(Message message);
        std::string generateClassBuilderConstructor(Message message);
        std::string generateClassBuilderHeader(Message message);
        std::string generateConstructor(Message message);
        std::string generateGetter(MessageField field, std::string className);
        std::string generateBuilderSetter(MessageField field, std::string className);
        std::string generateClear(Message message);
        std::string generateEmpty(Message message);
        std::string generateBuild(Message message);
        std::string getTypeFromField(MessageField field);
        static std::unordered_map<FieldType, std::string> typeMap;
};