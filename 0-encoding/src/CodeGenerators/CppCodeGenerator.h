#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "./CodeGenerator.h"
#include "../Model/Message.h"
#include "../Utils/StringUtils.h"

class CppCodeGenerator : public CodeGenerator {
    public:
        virtual std::vector<OutputFile> generateCode(std::unordered_map<std::string, Message> messages);
    protected:
        string generateHeader(Message message);
        string generateCpp(Message message);
        virtual string generateUtilHeader() = 0;
        virtual string generateUtilCpp() = 0;
        virtual string getIncludeHeaders() = 0;
        virtual string generateEncode(Message message) = 0;
        virtual string generateDecode(Message message) = 0;
        string generateClassHeader(Message message);
        string generateClassBuilderHeader(Message message);
        string generateConstructor(Message message);
        string generateGetter(MessageField field, string className);
        string generateBuilderSetter(MessageField field, string className);
        string generateClear(Message message);
        string generateBuild(Message message);
        string getTypeFromField(MessageField field);
        static unordered_map<FieldType, string> typeMap;
};