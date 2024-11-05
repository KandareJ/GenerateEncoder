#pragma once

#include "./CppCodeGenerator.h"

class JsonGenerator : public CppCodeGenerator {
    public:
        virtual string generateUtilHeader();
        virtual string generateUtilCpp();
    private:
        virtual string getIncludeHeaders();
        virtual string generateEncode(Message message);
        virtual string generateDecode(Message message);

};