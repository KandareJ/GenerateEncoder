#include "MessageField.h"

MessageField::MessageField(int index, string name, FieldType type) {
    this->index = index;
    this->name = name;
    this->type = type;
}

int MessageField::getIndex() {
    return index;
}

string MessageField::getName() {
    return name;
}

FieldType MessageField::getType() {
    return type;
}

string MessageField::toString() {
    ostringstream os;
    os << "<FIELD " << name << " " << index << ">";
    return os.str();
}

unordered_map<TokenType, FieldType> MessageFieldBuilder::typesMap = {
    {TOKEN_TYPE_STRING, FIELD_TYPE_STRING},
    {TOKEN_TYPE_INT32, FIELD_TYPE_INT32}
};

MessageFieldBuilder::MessageFieldBuilder() {
    this->clear();
}

MessageFieldBuilder* MessageFieldBuilder::setIndex(int index) {
    this->index = index;
    return this;
}

MessageFieldBuilder* MessageFieldBuilder::setName(string name) {
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

MessageFieldBuilder* MessageFieldBuilder::clear() {
    index = 0;
    name = "";
    type = FIELD_TYPE_UNSET;

    return this;
}

MessageField MessageFieldBuilder::build() {
    if (index >= 0 && name != "" && type != FIELD_TYPE_UNSET) {
        return MessageField(index, name, type);
    } else {
        throw MessageFieldBuilderError(index, name);
    }
}