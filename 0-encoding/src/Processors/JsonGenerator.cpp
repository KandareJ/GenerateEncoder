#include "JsonGenerator.h"

string JsonGenerator::getIncludeHeaders() {
    ostringstream os;
    os << "#include \"JsonUtils.h\"" << endl;
    return os.str();
}

string JsonGenerator::generateEncode(Message message) {
    ostringstream os;

    os << "string " << StringUtils::capitalize(message.getName()) << "::encode() {" << endl;
    os << "\tostringstream os;" << endl << endl;
    os << "\tos << \"{\";" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << "\tos << \"\\\"" << message.getFields().at(i).getName() << "\\\":\"" << " << ";
        os << "JsonUtils::toValue(get" << StringUtils::capitalize(message.getFields().at(i).getName());
        
        if (i < message.getFields().size() - 1) {
            os << "()) << \",\";" << endl;
        }
        else {
            os << "());" << endl;
        }

    }
    
    os << "\tos << \"}\";" << endl << endl;
    os << "\treturn os.str();" << endl;
    os << "}" << endl;

    return os.str();
}

string JsonGenerator::generateDecode(Message message) {
    ostringstream os;

    os << "void " << StringUtils::capitalize(message.getName()) << "::decode(string message) {" << endl;
    os << "\tJsonParser parser = JsonParser(message);" << endl;
    os << "\tunordered_map<string, string> map = parser.parse();" << endl << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        string fieldName = message.getFields().at(i).getName();

        if (message.getFields().at(i).getType() == FIELD_TYPE_STRING) {
            os << "\tthis->" << fieldName << " = map.at(\"" << fieldName << "\");" << endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_INT32) {
            os << "\tthis->" << fieldName << " = stoi(map.at(\"" << fieldName << "\"));" << endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_UINT32) {
            os << "\tthis->" << fieldName << " = stoul(map.at(\"" << fieldName << "\"));" << endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_DOUBLE) {
            os << "\tthis->" << fieldName << " = stod(map.at(\"" << fieldName << "\"));" << endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_UINT64) {
            os << "\tthis->" << fieldName << " = stoul(map.at(\"" << fieldName << "\"));" << endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_INT64) {
            os << "\tthis->" << fieldName << " = stol(map.at(\"" << fieldName << "\"));" << endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_BOOL) {
            os << "\tthis->" << fieldName << " = map.at(\"" << fieldName << "\") == \"true\";" << endl;
        }
    }

    os << "}" << endl;

    return os.str();
}

string JsonGenerator::generateUtilHeader() {
    return R"(#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

class JsonUtils {
    public:
        static string toValue(string value);
        static string toValue(int value);
        static string toValue(double value);
        static string toValue(long value);
        static string toValue(bool value);
        static string toValue(unsigned int value);
        static string toValue(unsigned long value);
};

class JsonParserState;

class JsonParser {
    public:
        JsonParser(string toParse);
        unordered_map<string, string> parse();
    private:
        friend class JsonParserState;
        void changeState(JsonParserState* nextState);
        string input;
        unordered_map<string, string> values;
        string currentKey;
        string currentValue;
        JsonParserState* state;
};

class JsonParserState {
    public:
        virtual void readCharacter(char character) = 0;
    protected:
        void changeState(JsonParserState* nextState);
        void appendToKey(char current);
        void appendToValue(char current);
        void push();
        JsonParser* parser;
};

class JsonInitialState : public JsonParserState {
    public:
        JsonInitialState(JsonParser* parser);
        virtual void readCharacter(char character);
};

class JsonPreReadKeyState : public JsonParserState {
    public:
        JsonPreReadKeyState(JsonParser* parser);
        virtual void readCharacter(char character);
};

class JsonReadKeyState : public JsonParserState {
    public:
        JsonReadKeyState(JsonParser* parser);
        virtual void readCharacter(char character);
};

class JsonPostReadKeyState : public JsonParserState {
    public:
        JsonPostReadKeyState(JsonParser* parser);
        virtual void readCharacter(char character);
};

class JsonPreReadValueState : public JsonParserState {
    public:
        JsonPreReadValueState(JsonParser* parser);
        virtual void readCharacter(char character);
};

class JsonReadValueState : public JsonParserState {
    public:
        JsonReadValueState(JsonParser* parser);
        virtual void readCharacter(char character);
};

class JsonPostReadValueState : public JsonParserState {
    public:
        JsonPostReadValueState(JsonParser* parser);
        virtual void readCharacter(char character);
};)";
}

string JsonGenerator::generateUtilCpp() {
    return R"(#include "JsonUtils.h"

string JsonUtils::toValue(string value) {
    ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

string JsonUtils::toValue(int value) {
    ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

string JsonUtils::toValue(double value) {
    ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

string JsonUtils::toValue(unsigned int value) {
    ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

string JsonUtils::toValue(unsigned long value) {
    ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

string JsonUtils::toValue(long value) {
    ostringstream os;
    os << "\"" << value << "\"";
    return os.str();
}

string JsonUtils::toValue(bool value) {
    return value ? "\"true\"" : "\"false\"";
}

JsonParser::JsonParser(string toParse) {
    input = toParse;
    state = new JsonInitialState(this);
}

unordered_map<string, string> JsonParser::parse() {
    for (int i = 0; i < input.length(); i++) {
        state->readCharacter(input.at(i));
    }

    return values;
}

void JsonParser::changeState(JsonParserState* nextState) {
    delete state;
    state = nextState;
}

void JsonParserState::changeState(JsonParserState* nextState) {
    parser->changeState(nextState);
}

void JsonParserState::appendToKey(char current) {
    parser->currentKey += current;
}

void JsonParserState::appendToValue(char current) {
    parser->currentValue += current;
}

void JsonParserState::push() {
    parser->values.insert({parser->currentKey, parser->currentValue});
    parser->currentKey = "";
    parser->currentValue = "";
}

JsonInitialState::JsonInitialState(JsonParser* parser) {
    this->parser = parser;
}

void JsonInitialState::readCharacter(char character) {
    if (character == '{') {
        changeState(new JsonPreReadKeyState(parser));
    }
}

JsonPreReadKeyState::JsonPreReadKeyState(JsonParser* parser) {
    this->parser = parser;
}

void JsonPreReadKeyState::readCharacter(char character) {
    if (character == '"') {
        changeState(new JsonReadKeyState(parser));
    }
}

JsonReadKeyState::JsonReadKeyState(JsonParser* parser) {
    this->parser = parser;
}

void JsonReadKeyState::readCharacter(char character) {
    if (character == '"') {
        changeState(new JsonPostReadKeyState(parser));
    }
    else {
        appendToKey(character);
    }
}

JsonPostReadKeyState::JsonPostReadKeyState(JsonParser* parser) {
    this->parser = parser;
}

void JsonPostReadKeyState::readCharacter(char character) {
    if (character == ':') {
        changeState(new JsonPreReadValueState(parser));
    }
}

JsonPreReadValueState::JsonPreReadValueState(JsonParser* parser) {
    this->parser = parser;
}

void JsonPreReadValueState::readCharacter(char character) {
    if (character == '"') {
        changeState(new JsonReadValueState(parser));
    }
}

JsonReadValueState::JsonReadValueState(JsonParser* parser) {
    this->parser = parser;
}

void JsonReadValueState::readCharacter(char character) {
    if (character == '"') {
        changeState(new JsonPostReadValueState(parser));
    }
    else {
        appendToValue(character);
    }
}

JsonPostReadValueState::JsonPostReadValueState(JsonParser* parser) {
    this->parser = parser;
}

void JsonPostReadValueState::readCharacter(char character) {
    if (character == ',') {
        push();
        changeState(new JsonPreReadKeyState(parser));
    }
    else if (character == '}') {
        push();
        changeState(new JsonInitialState(parser));
    }
}
)";
}