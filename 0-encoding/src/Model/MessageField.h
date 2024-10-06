#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "../Exceptions/BuilderError.h"
#include "Token.h"

using namespace std;

enum FieldType {
    FIELD_TYPE_UNSET,
    FIELD_TYPE_STRING,
    FIELD_TYPE_INT32
};

class MessageField {
    public:
        int getIndex();
        string getName();
        FieldType getType();
        string toString();
    private:
        friend class MessageFieldBuilder;
        MessageField(int index, string name, FieldType type);
        int index;
        string name;
        FieldType type;
};

class MessageFieldBuilder {
    public:
        MessageFieldBuilder();
        MessageFieldBuilder* setIndex(int index);
        MessageFieldBuilder* setName(string name);
        MessageFieldBuilder* setTypeFromToken(TokenType type);
        MessageFieldBuilder* clear();
        MessageField build();
    private:
        int index;
        string name;
        FieldType type;
        static unordered_map<TokenType, FieldType> typesMap;
};

