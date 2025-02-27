#include "CodeGenerators/JsonGenerator.h"

std::string JsonGenerator::getIncludeHeaders() {
    std::ostringstream os;
    os << "#include \"JsonUtils.h\"" << std::endl;
    return os.str();
}

std::string JsonGenerator::generateEncode(Message message) {
    std::ostringstream os;

    os << "std::string " << StringUtils::capitalize(message.getName()) << "::encode() {" << std::endl;
    os << "\tstd::ostringstream os;" << std::endl << std::endl;
    os << "\tos << \"{\";" << std::endl;

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        os << "\tos << \"\\\"" << message.getFields().at(i).getName() << "\\\":\"" << " << ";
        os << generateEncodeField(message.getFields().at(i));
        
        if (i < message.getFields().size() - 1) {
            os << " << \",\";" << std::endl;
        }
        else {
            os << ";" << std::endl;
        }

    }
    
    os << "\tos << \"}\";" << std::endl << std::endl;
    os << "\treturn os.str();" << std::endl;
    os << "}" << std::endl;

    return os.str();
}

std::string JsonGenerator::generateEncodeField(MessageField field) {
    std::ostringstream os;

    if (field.isList()) {
        os << "\"[\";" << std::endl;

        os << "\tfor (unsigned int i = 0; i < " << field.getName() << ".size(); i++) {" << std::endl;
        os << "\t\tos << ";

        if (field.getType() == FIELD_TYPE_CUSTOM) {
            os << field.getName() << ".at(i).encode();" << std::endl;
        }
        else if (field.getType() == FIELD_TYPE_BYTES) {
            os << "JsonUtils::toValue(JsonUtils::encodeBase64(" << field.getName() << "))";
        }
        else {
            os << "JsonUtils::toValue(" << field.getName() << ".at(i));" << std::endl;
        }

        os << "\t\tif (i < " << field.getName() << ".size() - 1) {" << std::endl;
        os << "\t\t\tos << \",\";" << std::endl;
        os << "\t\t}" << std::endl;

        os << "\t}" << std::endl;

        os << "\tos << \"]\"";
    }
    else {
        if (field.getType() == FIELD_TYPE_CUSTOM) {
            os << field.getName() << ".encode()";
        }
        else if (field.getType() == FIELD_TYPE_BYTES) {
            os << "JsonUtils::toValue(JsonUtils::encodeBase64(" << field.getName() << "))";
        }
        else {
            os << "JsonUtils::toValue(" << field.getName() << ")";

        }
    }

    return os.str();
}

std::string JsonGenerator::generateDecodeField(MessageField field) {   
    std::ostringstream os;

    if (field.getType() == FIELD_TYPE_STRING) {
        os << "((JsonValue*) message->getValue(\"" << field.getName() << "\"))->getValue()";
    }
    else if (field.getType() == FIELD_TYPE_INT32) {
        os << "stoi(((JsonValue*) message->getValue(\"" << field.getName() << "\"))->getValue())";
    }
    else if (field.getType() == FIELD_TYPE_UINT32) {
        os << "stoul(((JsonValue*) message->getValue(\"" << field.getName() << "\"))->getValue())";
    }
    else if (field.getType() == FIELD_TYPE_DOUBLE) {
        os << "stod(((JsonValue*) message->getValue(\"" << field.getName() << "\"))->getValue())";
    }
    else if (field.getType() == FIELD_TYPE_UINT64) {
        os << "stoul(((JsonValue*) message->getValue(\"" << field.getName() << "\"))->getValue())";
    }
    else if (field.getType() == FIELD_TYPE_INT64) {
        os << "stol(((JsonValue*) message->getValue(\"\"" << field.getName() << "\"))->getValue())";
    }
    else if (field.getType() == FIELD_TYPE_BOOL) {
        os << "((JsonValue*)message->getValue(\"" << field.getName() << "\"))->getValue() == \"true\"";
    }
    else if (field.getType() == FIELD_TYPE_BYTES) {
        os << "JsonUtils::decodeBase64(((JsonValue*) message->getValue(\"" << field.getName() << "\"))->getValue())";
    }
   return os.str();
}

std::string JsonGenerator::generateDecode(Message message) {
    std::ostringstream os;

    os << "void " << StringUtils::capitalize(message.getName()) << "::decode(std::string message) {" << std::endl;
    os << "\tJsonParser parser = JsonParser();" << std::endl;
    os << "\treturn decode((JsonMap*)parser.parse(message));" << std::endl;
    os << "}" << std::endl << std::endl;

    os << "void " << StringUtils::capitalize(message.getName()) << "::decode(JsonMap* message) {" << std::endl;

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        MessageField field = message.getFields().at(i);

        if (field.isList()) {
            os << "\tthis->" << field.getName() << ".clear();" << std::endl;
            os << "\tstd::vector<JsonObject*> " << field.getName() << "Items = ((JsonList*) message->getValue(\"" << field.getName() << "\"))->getList();" << std::endl;
            os << "\tfor (unsigned int i = 0; i < " << field.getName() << "Items.size(); i++) {" << std::endl;

            if (field.getType() == FIELD_TYPE_CUSTOM) {
                os << "\t\t" << field.getCustomType() << "Builder builder;" << std::endl;
                os << "\t\t" << field.getCustomType() << " customType = builder.build();" << std::endl;
                os << "\t\tcustomType.decode((JsonMap*) " << field.getName() << "Items.at(i));" << std::endl;
                os << "\t\tthis->" << field.getName() << ".push_back(customType);" << std::endl;
            }
            else {
                os << "\t\tthis->" << field.getName() << ".push_back(";

                if (field.getType() == FIELD_TYPE_STRING) {
                    os << "((JsonValue*) " << field.getName() << "Items.at(i))->getValue()";
                }
                else if (field.getType() == FIELD_TYPE_INT32) {
                    os << "stoi(((JsonValue*) " << field.getName() << "Items.at(i))->getValue())";
                }
                else if (field.getType() == FIELD_TYPE_UINT32) {
                    os << "stoul(((JsonValue*) " << field.getName() << "Items.at(i))->getValue())";
                }
                else if (field.getType() == FIELD_TYPE_DOUBLE) {
                    os << "stod(((JsonValue*) " << field.getName() << "Items.at(i))->getValue())";
                }
                else if (field.getType() == FIELD_TYPE_UINT64) {
                    os << "stoul(((JsonValue*) " << field.getName() << "Items.at(i))->getValue())";
                }
                else if (field.getType() == FIELD_TYPE_INT64) {
                    os << "stol(((JsonValue*) " << field.getName() << "Items.at(i))->getValue())";
                }
                else if (field.getType() == FIELD_TYPE_BOOL) {
                    os << "((JsonValue*) " << field.getName() << "Items.at(i))->getValue() == \"true\"";
                }
                else if (field.getType() == FIELD_TYPE_BYTES) {
                    os << "JsonUtils::decodeBase64(((JsonValue*) " << field.getName() << "Items.at(i))->getValue())";
                }

                os << ");" << std::endl;
            }
            
            os << "\t}" << std::endl;
        }
        else {
            if (field.getType() == FIELD_TYPE_CUSTOM) {
                os << "\tthis->" << field.getName() << ".decode((JsonMap*)message->getValue(\"" << field.getName() << "\"));" << std::endl;
            }
            else {
                os << "\tthis->" << field.getName() << " = " << generateDecodeField(field) << ";" << std::endl;
            }
        }
    }

    os << "}" << std::endl;

    return os.str();
}

std::string JsonGenerator::generateAdditionalPublic(Message message) {
    std::ostringstream os;
    
    os << "\t\tvoid decode(JsonMap* message);" << std::endl;

    return os.str();
}

std::string JsonGenerator::generateUtilHeader() {
    return R"(#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>

class JsonUtils {
    public:
        static bool isDigit(char character);
        static bool isWhitespace(char character);
        static std::string toValue(std::string value);
        static std::string toValue(int value);
        static std::string toValue(double value);
        static std::string toValue(long value);
        static std::string toValue(bool value);
        static std::string toValue(unsigned int value);
        static std::string toValue(unsigned long value);
        static std::string encodeBase64(std::string bytes);
        static std::string decodeBase64(std::string base64Bytes);
    private:
        static std::string base64CharSet;
        static unsigned int getPosInCharSet(char character);
};

class JsonParseError {
    public:
        JsonParseError(std::string input, int index);
        std::string getMessage();
    private:
        std::string message;
};

class JsonObject {};

class JsonList : public JsonObject {
    public:
        void appendValue(JsonObject* value);
        std::vector<JsonObject*> getList();
    private:
        std::vector<JsonObject*> list;
};

class JsonMap : public JsonObject {
    public:
        void insert(JsonObject* value);
        void appendToKey(char character);
        JsonObject* getValue(std::string key);
        std::unordered_map<std::string, JsonObject*> getMap();
    private:
        std::unordered_map<std::string, JsonObject*> map;
        std::ostringstream key;
};

class JsonValue : public JsonObject {
    public:
        void appendToValue(char character);
        std::string getValue();
    private:
        std::ostringstream value;
};

class JsonParserState;

class JsonParser {
    public:
        JsonParser();
        ~JsonParser();
        JsonObject* parse(std::string input, int& index);
        JsonObject* parse(std::string input);
    private:
        friend class JsonParserState;
        void changeState(JsonParserState* nextState);
        void setCurrent(JsonObject* current);
        JsonObject* getCurrent();
        JsonParserState* state;
        JsonObject* current;
};

class JsonParserState {
    public:
        JsonParserState(JsonParser* parser);
        virtual void readCharacter(std::string input, int& index) = 0;
        virtual bool isTerminalState();
    protected:
        void changeState(JsonParserState* nextState);
        void setCurrent(JsonObject* current);
        JsonObject* getCurrent();
        JsonParser* parser;
};

class JsonParserInitialState : public JsonParserState {
    public:
        JsonParserInitialState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
};

class JsonParserObjectState : public JsonParserState {
    public:
        JsonParserObjectState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
};

class JsonParserKeyState : public JsonParserState {
    public:
        JsonParserKeyState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
};

class JsonParserPostKeyState : public JsonParserState {
    public:
        JsonParserPostKeyState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
};

class JsonParserObjectValueState : public JsonParserState {
    public:
        JsonParserObjectValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
};

class JsonParserPostObjectValueState : public JsonParserState {
    public:
        JsonParserPostObjectValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
};

class JsonParserFinalState : public JsonParserState {
    public:
        JsonParserFinalState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
        virtual bool isTerminalState();
};

class JsonParserStringValueState : public JsonParserState {
    public:
        JsonParserStringValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
};

class JsonParserNumericValueState : public JsonParserState {
    public:
        JsonParserNumericValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
};

class JsonParserDecimalValueState : public JsonParserState {
    public:
        JsonParserDecimalValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
};

class JsonParserListValueState : public JsonParserState {
    public:
        JsonParserListValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
};

class JsonParserPostListValueState : public JsonParserState {
    public:
        JsonParserPostListValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, int& index);
};
)";
}

std::string JsonGenerator::generateUtilCpp() {
    return R"(#include "JsonUtils.h"

bool JsonUtils::isDigit(char character) {
    return character >= '0' && character <= '9';
}

bool JsonUtils::isWhitespace(char character) {
    return character == ' ' || character == '\t' || character == '\r' || character == '\n';
}

std::string JsonUtils::toValue(std::string value) {
    std::ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

std::string JsonUtils::toValue(int value) {
    std::ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

std::string JsonUtils::toValue(double value) {
    std::ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

std::string JsonUtils::toValue(unsigned int value) {
    std::ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

std::string JsonUtils::toValue(unsigned long value) {
    std::ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

std::string JsonUtils::toValue(long value) {
    std::ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

std::string JsonUtils::toValue(bool value) {
    return value ? "\"true\"" : "\"false\"";
}

std::string JsonUtils::base64CharSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

unsigned int JsonUtils::getPosInCharSet(char character) {
    if (character >= 'A' && character <= 'Z') {
        return character - 'A';
    }
    else if (character >= 'a' && character <= 'z') {
        return character - 'a' + ('Z' - 'A') + 1;
    }
    else if (character >= '0' && character <= '9') {
        return character - '0' + ('Z' - 'A') + ('z' - 'a') + 2;
    }
    else if (character == '+' || character == '-') {
        return 62;
    }
    else {
        return 63;
    }
}

std::string JsonUtils::encodeBase64(std::string bytes) {
    std::string encoded;

    for (unsigned int i = 0; i < bytes.size(); i+=3) {
        encoded.push_back(base64CharSet[(bytes.at(i) & 0xfc) >> 2]);

        if (i+1 < bytes.size()) {
            encoded.push_back(base64CharSet[((bytes[i] & 0x03) << 4 ) + ((bytes[i+1] & 0xf0) >> 4)]);

            if (i+2 < bytes.size()) {
                encoded.push_back(base64CharSet[((bytes[i+1] & 0x0f) << 2) + ((bytes[i+2] & 0xc0) >> 6)]);
                encoded.push_back(base64CharSet[  bytes[i+2] & 0x3f]);
            }
            else {
                encoded.push_back(base64CharSet[(bytes[i+1] & 0x0f) << 2]);
                encoded.push_back('=');
            }
        }
        else {
            encoded.push_back(base64CharSet[(bytes[i+1] & 0x03) << 4]);
            encoded.push_back('=');
            encoded.push_back('=');
        }

    }

    return encoded;
}

std::string JsonUtils::decodeBase64(std::string base64Bytes) {
    std::string decoded;

    for (unsigned int i = 0; i < base64Bytes.length(); i+=4) {
       int char1Pos = getPosInCharSet(base64Bytes.at(i+1) );
       decoded.push_back(static_cast<std::string::value_type>(((getPosInCharSet(base64Bytes.at(i))) << 2) + ((char1Pos & 0x30) >> 4)));

       if ((i + 2 < base64Bytes.length()) && base64Bytes.at(i+2) != '=') {
          int char2Pos = getPosInCharSet(base64Bytes.at(i+2) );
          decoded.push_back(static_cast<std::string::value_type>(((char1Pos & 0x0f) << 4) + ((char2Pos & 0x3c) >> 2)));

          if (i + 3 < base64Bytes.length() && base64Bytes.at(i+3) != '=') {
             decoded.push_back(static_cast<std::string::value_type>(((char2Pos & 0x03 ) << 6) + getPosInCharSet(base64Bytes.at(i+3))));
          }
       }
    }

    return decoded;
}

JsonParseError::JsonParseError(std::string input, int index) {
    std::ostringstream os;
    os << "JsonParseError: Unexpected token " << input.at(index);
    os << " at index " << index << std::endl;
    this->message = os.str();
}

std::string JsonParseError::getMessage() {
    return message;
}

void JsonList::appendValue(JsonObject* value) {
    list.push_back(value);
}

std::vector<JsonObject*> JsonList::getList() {
    return list;
}

void JsonMap::insert(JsonObject* value) {
    map.insert({key.str(), value});
    key.str("");
}

void JsonMap::appendToKey(char character) {
    key << character;
}

JsonObject* JsonMap::getValue(std::string key) {
    try {
        return map.at(key);
    } catch (...) {
        return new JsonValue();
    }
}

std::unordered_map<std::string, JsonObject*> JsonMap::getMap() {
    return map;
}

void JsonValue::appendToValue(char character) {
    value << character;
}

std::string JsonValue::getValue() {
    return value.str();
}

JsonParser::JsonParser() {
    state = new JsonParserInitialState(this);
}

JsonParser::~JsonParser() {
    delete state;
}

JsonObject* JsonParser::parse(std::string input, int& index) {
    while (!state->isTerminalState() && index < input.length()) {
        state->readCharacter(input, index);
    }

    return current;
}

JsonObject* JsonParser::parse(std::string input) {
    int index = 0;
    return parse(input, index);
}

void JsonParser::changeState(JsonParserState* nextState) {
    delete state;
    state = nextState;
}

void JsonParser::setCurrent(JsonObject* current) {
    this->current = current;
}

JsonObject* JsonParser::getCurrent() {
    return current;
}

JsonParserState::JsonParserState(JsonParser* parser) {
    this->parser = parser;
}

bool JsonParserState::isTerminalState() {
    return false;
}

void JsonParserState::setCurrent(JsonObject* current) {
    parser->current = current;
}

JsonObject* JsonParserState::getCurrent() {
    return parser->current;
}

void JsonParserState::changeState(JsonParserState* nextState) {
    parser->changeState(nextState);
}

void JsonParserInitialState::readCharacter(std::string input, int& index) {
    if (JsonUtils::isDigit(input.at(index))) {
        setCurrent(new JsonValue());
        ((JsonValue*) getCurrent())->appendToValue(input.at(index));
        changeState(new JsonParserNumericValueState(parser));
    }
    else if (JsonUtils::isWhitespace(input.at(index))) {
    }
    else if (input.at(index) == '[') {
        setCurrent(new JsonList());
        changeState(new JsonParserListValueState(parser));
    }
    else if (input.at(index) == '\"') {
        setCurrent(new JsonValue());
        changeState(new JsonParserStringValueState(parser));
    }
    else if (input.at(index) == '{') {
        setCurrent(new JsonMap());
        changeState(new JsonParserObjectState(parser));
    }
    else {
        throw JsonParseError(input, index);
    }
    index++;
}

void JsonParserObjectState::readCharacter(std::string input, int& index) {
    if (JsonUtils::isWhitespace(input.at(index))) {
    }
    else if (input.at(index) == '\"') {
        changeState(new JsonParserKeyState(parser));
    }
    else {
        throw JsonParseError(input, index);
    }
    index++;
}

void JsonParserKeyState::readCharacter(std::string input, int& index) {
    if (input.at(index) == '\"') {
        changeState(new JsonParserPostKeyState(parser));
    }
    else {
        ((JsonMap*) getCurrent())->appendToKey(input.at(index));
    }
    index++;
}

void JsonParserPostKeyState::readCharacter(std::string input, int& index) {
    if (JsonUtils::isWhitespace(input.at(index))) {
    }
    else if (input.at(index) == ':') {
        changeState(new JsonParserObjectValueState(parser));
    }
    else {
        throw JsonParseError(input, index);
    }
    index++;
}

void JsonParserObjectValueState::readCharacter(std::string input, int& index) {
    JsonParser recursiveParser;
    ((JsonMap*) getCurrent())->insert(recursiveParser.parse(input, index));
    changeState(new JsonParserPostObjectValueState(parser));
}

void JsonParserPostObjectValueState::readCharacter(std::string input, int& index) {
    if (JsonUtils::isWhitespace(input.at(index))) {

    }
    else if (input.at(index) == '}') {
        changeState(new JsonParserFinalState(parser));
    }
    else if (input.at(index) == ',') {
        changeState(new JsonParserObjectState(parser));
    }
    else {
        throw JsonParseError(input, index);
    }
    index++;
}

void JsonParserFinalState::readCharacter(std::string input, int& index) {
    if (JsonUtils::isWhitespace(input.at(index))) {

    }
    else {
        throw JsonParseError(input, index);
    }
    index++;
}

bool JsonParserFinalState::isTerminalState() {
    return true;
}

void JsonParserStringValueState::readCharacter(std::string input, int& index) {
    if (input.at(index) == '\"') {
        changeState(new JsonParserFinalState(parser));
    }
    else {
        ((JsonValue*) getCurrent())->appendToValue(input.at(index));
    }
    index++;
}

void JsonParserNumericValueState::readCharacter(std::string input, int& index) {
    if (JsonUtils::isDigit(input.at(index))) {
        ((JsonValue*) getCurrent())->appendToValue(input.at(index));
        index++;
    }
    else if (input.at(index) == '.') {
        ((JsonValue*) getCurrent())->appendToValue(input.at(index));
        changeState(new JsonParserDecimalValueState(parser));
        index++;
    }
    else {
        changeState(new JsonParserFinalState(parser));
    }
}

void JsonParserDecimalValueState::readCharacter(std::string input, int& index) {
    if (JsonUtils::isDigit(input.at(index))) {
        ((JsonValue*) getCurrent())->appendToValue(input.at(index));
        index++;
    }
    else {
        changeState(new JsonParserFinalState(parser));
    }
}

void JsonParserListValueState::readCharacter(std::string input, int& index) {
    JsonParser recursiveParser;
    ((JsonList*) getCurrent())->appendValue(recursiveParser.parse(input, index));
    changeState(new JsonParserPostListValueState(parser));
}

void JsonParserPostListValueState::readCharacter(std::string input, int& index) {
    if (JsonUtils::isWhitespace(input.at(index))) {

    }
    else if (input.at(index) == ',') {
        changeState(new JsonParserListValueState(parser));
    }
    else if (input.at(index) == ']') {
        changeState(new JsonParserFinalState(parser));
    }
    else {
        throw JsonParseError(input, index);
    }
    index++;
}
)";
}

std::string JsonGenerator::getUtilFileName() {
    return "JsonUtils";
}