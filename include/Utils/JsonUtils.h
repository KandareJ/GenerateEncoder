#pragma once

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
        JsonParseError(std::string input, unsigned int index);
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
        JsonObject* parse(std::string input, unsigned int& index);
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
        virtual ~JsonParserState();
        virtual void readCharacter(std::string input, unsigned int& index) = 0;
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
        virtual void readCharacter(std::string input, unsigned int& index);
};

class JsonParserObjectState : public JsonParserState {
    public:
        JsonParserObjectState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, unsigned int& index);
};

class JsonParserKeyState : public JsonParserState {
    public:
        JsonParserKeyState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, unsigned int& index);
};

class JsonParserPostKeyState : public JsonParserState {
    public:
        JsonParserPostKeyState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, unsigned int& index);
};

class JsonParserObjectValueState : public JsonParserState {
    public:
        JsonParserObjectValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, unsigned int& index);
};

class JsonParserPostObjectValueState : public JsonParserState {
    public:
        JsonParserPostObjectValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, unsigned int& index);
};

class JsonParserFinalState : public JsonParserState {
    public:
        JsonParserFinalState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, unsigned int& index);
        virtual bool isTerminalState();
};

class JsonParserStringValueState : public JsonParserState {
    public:
        JsonParserStringValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, unsigned int& index);
};

class JsonParserNumericValueState : public JsonParserState {
    public:
        JsonParserNumericValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, unsigned int& index);
};

class JsonParserDecimalValueState : public JsonParserState {
    public:
        JsonParserDecimalValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, unsigned int& index);
};

class JsonParserListValueState : public JsonParserState {
    public:
        JsonParserListValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, unsigned int& index);
};

class JsonParserPostListValueState : public JsonParserState {
    public:
        JsonParserPostListValueState(JsonParser* parser) : JsonParserState(parser) {};
        virtual void readCharacter(std::string input, unsigned int& index);
};