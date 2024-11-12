#include "MessageField.h"

MessageField::MessageField(int index, std::string name, FieldType type, bool isList, std::string customType) {
    this->index = index;
    this->name = name;
    this->type = type;
    this->_isList = isList;
    this->customType = customType;
}

int MessageField::getIndex() {
    return index;
}

std::string MessageField::getName() {
    return name;
}

FieldType MessageField::getType() {
    return type;
}

std::string MessageField::getCustomType() {
    return customType;
}

bool MessageField::isList() {
    return _isList;
}

std::string MessageField::toString() {
    std::ostringstream os;
    os << "<FIELD " << name;
    if (_isList) {
        os << "[]";
    }
    os << " " << index << ">";
    return os.str();
}

std::unordered_map<TokenType, FieldType> MessageFieldBuilder::typesMap = {
    {TOKEN_TYPE_STRING, FIELD_TYPE_STRING},
    {TOKEN_TYPE_INT32, FIELD_TYPE_INT32},
    {TOKEN_TYPE_UINT32, FIELD_TYPE_UINT32},
    {TOKEN_TYPE_DOUBLE, FIELD_TYPE_DOUBLE},
    {TOKEN_TYPE_BYTES, FIELD_TYPE_BYTES},
    {TOKEN_TYPE_UINT64, FIELD_TYPE_UINT64},
    {TOKEN_TYPE_INT64, FIELD_TYPE_INT64},
    {TOKEN_TYPE_BOOL, FIELD_TYPE_BOOL}
};

MessageFieldBuilder::MessageFieldBuilder() {
    this->clear();
}

MessageFieldBuilder* MessageFieldBuilder::setIndex(int index) {
    this->index = index;
    return this;
}

MessageFieldBuilder* MessageFieldBuilder::setName(std::string name) {
    this->name = name;
    return this;
}

MessageFieldBuilder* MessageFieldBuilder::setTypeFromToken(TokenType type) {
    try {
        this->type = typesMap.at(type);
    } catch (...) {
        this->type = FIELD_TYPE_UNSET;
    }

    return this;
}

MessageFieldBuilder* MessageFieldBuilder::setCustomType(std::string type) {
    this->customType = type;
    this->type = FIELD_TYPE_CUSTOM;

    return this;
}

MessageFieldBuilder* MessageFieldBuilder::setIsList(bool isList) {
    this->isList = isList;
    return this;
}

MessageFieldBuilder* MessageFieldBuilder::clear() {
    index = 0;
    name = "";
    type = FIELD_TYPE_UNSET;
    isList = false;

    return this;
}

MessageField MessageFieldBuilder::build() {
    if (index >= 0 && name != "" && type != FIELD_TYPE_UNSET) {
        return MessageField(index, name, type, isList, customType);
    } else {
        throw MessageFieldBuilderError(index, name);
    }
}