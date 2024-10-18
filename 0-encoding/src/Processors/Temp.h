#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "../Model/Message.h"
#include "../Utils/StringUtils.h"

class Temp {
    public:
        string generateHeader(Message message);
        string generateCpp(Message message);
        string generateUtilHeader();
        string generateUtilCpp();
    private:
        string generateClassHeader(Message message);
        string generateClassBuilderHeader(Message message);
        string generateConstructor(Message message);
        string generateEncode(Message message);
        string generateDecode(Message message);
        string generateGetter(MessageField field, string className);
        string generateBuilderSetters(MessageField field, string className);
        string generateBuild(Message message);
        static unordered_map<FieldType, string> typeMap;
};