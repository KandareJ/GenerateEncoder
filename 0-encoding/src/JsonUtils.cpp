#include "JsonUtils.h"

bool JsonUtils::isDigit(char character) {
    return character >= '0' && character <= '9';
}

bool JsonUtils::isCharacter(char character) {
    return (character >= 'A' && character <= 'Z')
        || (character >= 'a' && character <= 'z');
}

bool JsonUtils::isWhitespace(char character) {
    return character == ' ' || character == '\t' || character == '\r' || character == '\n';
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