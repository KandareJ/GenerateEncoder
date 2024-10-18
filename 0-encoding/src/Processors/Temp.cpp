#include "Temp.h"

unordered_map<FieldType, string> Temp::typeMap = {
        {FIELD_TYPE_STRING, "string"},
        {FIELD_TYPE_INT32, "int"},
    };

string Temp::generateHeader(Message message) {
    ostringstream os;

    os << "#pragma once" << endl << endl;
    os << "#include <sstream>" << endl;
    os << "#include <string>" << endl << endl;
    os << "#include \"JsonUtils.h\"" << endl << endl;
    os << "using namespace std;" << endl << endl;
    os << generateClassHeader(message) << endl;
    os << generateClassBuilderHeader(message) << endl;
    

    return os.str();
}

string Temp::generateClassHeader(Message message) {
    ostringstream os;

    os << "class " << StringUtils::capitalize(message.getName()) << " {" << endl;
    os << "\tpublic:" << endl;
    os << "\t\tstring encode();" << endl;
    os << "\t\tvoid decode(string message);" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << "\t\t" << typeMap.at(message.getFields().at(i).getType());
        os << " get" << StringUtils::capitalize(message.getFields().at(i).getName()) << "();" << endl;
    }

    os << "\tprivate:" << endl;
    os << "\t\tfriend class " << StringUtils::capitalize(message.getName()) << "Builder;" << endl;
    os << "\t\t"<< StringUtils::capitalize(message.getName()) << "(";

    for (int i = 0; i < message.getFields().size(); i++) {
        os << typeMap.at(message.getFields().at(i).getType());
        os << " " << message.getFields().at(i).getName();
        
        if (i < message.getFields().size() - 1) {
            os << ", ";
        }
    }

    os << ");" << endl;
    
    for (int i = 0; i < message.getFields().size(); i++) {
        os << "\t\t" << typeMap.at(message.getFields().at(i).getType());
        os << " " << message.getFields().at(i).getName() << ";" << endl;
    }

    os << "};" << endl;

    return os.str();
}

string Temp::generateClassBuilderHeader(Message message) {
    ostringstream os;
    string className = StringUtils::capitalize(message.getName()) + "Builder";

    os << "class " << className << " {" << endl;
    os << "\tpublic:" << endl;
    os << "\t\t" << StringUtils::capitalize(message.getName()) << " build();" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << "\t\t" << className << "* set";
        os << StringUtils::capitalize(message.getFields().at(i).getName()) << "(";
        os << typeMap.at(message.getFields().at(i).getType()) << " ";
        os << message.getFields().at(i).getName() << ");" << endl;
    }

    os << "\tprivate:" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << "\t\t" << typeMap.at(message.getFields().at(i).getType());
        os << " " << message.getFields().at(i).getName() << ";" << endl;
    }

    os << "};" << endl;

    return os.str();
}

string Temp::generateCpp(Message message) {
    ostringstream os;
    string className = StringUtils::capitalize(message.getName());

    os << "#include \"" << message.getName() <<".h\"" << endl << endl;
    os << generateConstructor(message) << endl;
    os << generateEncode(message) << endl;
    os << generateDecode(message) << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << generateGetter(message.getFields().at(i), className) << endl;
    }

    os << generateBuild(message) << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << generateBuilderSetters(message.getFields().at(i), className) << endl;
    }

    return os.str();
}

string Temp::generateConstructor(Message message) {
    ostringstream os;

    os << StringUtils::capitalize(message.getName()) << "::";
    os << StringUtils::capitalize(message.getName()) << "(";

    for (int i = 0; i < message.getFields().size(); i++) {
        os << typeMap.at(message.getFields().at(i).getType());
        os << " " << message.getFields().at(i).getName();
        
        if (i < message.getFields().size() - 1) {
            os << ", ";
        }
    }

    os << ") {" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << "\tthis->" << message.getFields().at(i).getName();
        os << " = " << message.getFields().at(i).getName() << ";" << endl;
    }

    os << "}" << endl;

    return os.str();
}

string Temp::generateEncode(Message message) {
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

string Temp::generateDecode(Message message) {
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
    }

    os << "}" << endl;

    return os.str();
}

string Temp::generateGetter(MessageField field, string className) {
    ostringstream os;
    
    os << typeMap.at(field.getType()) << " " << className << "::";
    os << "get" << StringUtils::capitalize(field.getName()) << "() {" << endl;
    os << "\treturn " << field.getName() << ";" << endl;
    os << "}" << endl;

    return os.str();
}

string Temp::generateBuilderSetters(MessageField field, string className) {
    ostringstream os;
    string builderClassName = className + "Builder";
    
    os << builderClassName << "* " << builderClassName << "::";
    os << "set" << StringUtils::capitalize(field.getName()) << "(";
    os << typeMap.at(field.getType()) << " " << field.getName() << ") {" << endl;
    os << "\tthis->" << field.getName() << " = " << field.getName() << ";" << endl;
    os << "\treturn this;" << endl;
    os << "}" << endl;

    return os.str();
}

string Temp::generateBuild(Message message) {
    ostringstream os;
    string className = StringUtils::capitalize(message.getName());

    os << className << " " << className << "Builder::build() {" << endl;
    os << "\treturn " << className << "(";

    for (int i = 0; i < message.getFields().size(); i++) {
        os << message.getFields().at(i).getName();
        if (i < message.getFields().size() - 1) {
            os << ", ";
        }
    }

    os << ");" << endl;
    os << "}" << endl;
    
    return os.str();
}

string Temp::generateUtilHeader() {
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

string Temp::generateUtilCpp() {
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