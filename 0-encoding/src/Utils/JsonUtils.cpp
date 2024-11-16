#include "Utils/JsonUtils.h"

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

    for (int i = 0; i < bytes.size(); i+=3) {
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

    for (int i = 0; i < base64Bytes.length(); i+=4) {
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