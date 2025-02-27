#pragma once

#include "CodeGenerators/CppCodeGenerator.h"

class JbufGenerator : public CppCodeGenerator {
    public:
        virtual std::string generateUtilHeader();
        virtual std::string generateUtilCpp();
        virtual std::string getUtilFileName();
    private:
        virtual std::string getIncludeHeaders();
        virtual std::string generateEncode(Message message);
        std::string generateEncodeField(MessageField field);
        virtual std::string generateDecode(Message message);
};