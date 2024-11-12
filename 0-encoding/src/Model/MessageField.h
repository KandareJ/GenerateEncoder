#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "../Exceptions/BuilderError.h"
#include "Token.h"

enum FieldType {
    FIELD_TYPE_UNSET,
    FIELD_TYPE_STRING,
    FIELD_TYPE_INT32,
    FIELD_TYPE_UINT32,
    FIELD_TYPE_DOUBLE,
    FIELD_TYPE_BYTES,
    FIELD_TYPE_BOOL,
    FIELD_TYPE_UINT64,
    FIELD_TYPE_INT64,
    FIELD_TYPE_CUSTOM
};

class MessageField {
    public:
        int getIndex();
        std::string getName();
        FieldType getType();
        std::string getCustomType();
        bool isList();
        std::string toString();
    private:
        friend class MessageFieldBuilder;
        MessageField(int index, std::string name, FieldType type, bool isList, std::string customType);
        int index;
        std::string name;
        std::string customType;
        FieldType type;
        bool _isList;
};

class MessageFieldBuilder {
    public:
        MessageFieldBuilder();
        MessageFieldBuilder* setIndex(int index);
        MessageFieldBuilder* setName(std::string name);
        MessageFieldBuilder* setTypeFromToken(TokenType type);
        MessageFieldBuilder* setCustomType(std::string type);
        MessageFieldBuilder* setIsList(bool isList);
        MessageFieldBuilder* clear();
        MessageField build();
    private:
        int index;
        std::string name;
        FieldType type;
        std::string customType;
        bool isList;
        static std::unordered_map<TokenType, FieldType> typesMap;
};

