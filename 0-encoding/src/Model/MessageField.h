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
    FIELD_TYPE_INT32,
    FIELD_TYPE_UINT32,
    FIELD_TYPE_DOUBLE,
    FIELD_TYPE_BYTES,
    FIELD_TYPE_BOOL,
    FIELD_TYPE_UINT64,
    FIELD_TYPE_INT64
};

class MessageField {
    public:
        int getIndex();
        string getName();
        FieldType getType();
        bool getIsList();
        string toString();
    private:
        friend class MessageFieldBuilder;
        MessageField(int index, string name, FieldType type, bool isList);
        int index;
        string name;
        FieldType type;
        bool isList;
};

class MessageFieldBuilder {
    public:
        MessageFieldBuilder();
        MessageFieldBuilder* setIndex(int index);
        MessageFieldBuilder* setName(string name);
        MessageFieldBuilder* setTypeFromToken(TokenType type);
        MessageFieldBuilder* setIsList(bool isList);
        MessageFieldBuilder* clear();
        MessageField build();
    private:
        int index;
        string name;
        FieldType type;
        bool isList;
        static unordered_map<TokenType, FieldType> typesMap;
};

