#include "Processors/Parser.h"

Parser::Parser() {
    state = new ParserBaseState(this);
    messageFieldBuilder = new MessageFieldBuilder();
    messageBuilder = new MessageBuilder();
}

Parser::~Parser() {
    delete state;
    delete messageFieldBuilder;
    delete messageBuilder;
}

std::unordered_map<std::string, Message> Parser::parse(std::vector<Token> tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        state->handleToken(tokens.at(i));
    }

    if (!state->isTerminalState()) {
         throw ParseError("Unexpected end of file");
    }

    return messages;
}

void Parser::changeState(ParserState* newState) {
    delete state;
    state = newState;
}

bool ParserState::isTerminalState() {
    return false;
}

void ParserState::changeState(ParserState* newState) {
    parser->changeState(newState);
}

MessageFieldBuilder* ParserState::getFieldBuilder() {
    return parser->messageFieldBuilder;
}

MessageBuilder* ParserState::getMessageBuilder() {
    return parser->messageBuilder;
}

void ParserState::addMessage(Message message) {
    parser->messages.insert({message.getName(), message});
}

void ParserState::insertCustomType(std::string type) {
    parser->customTypes.insert(type);
}

bool ParserState::containsCustomType(std::string type) {
        return parser->customTypes.find(type) != parser->customTypes.end();
}

ParserBaseState::ParserBaseState(Parser* parser) {
    this->parser = parser;
}

void ParserBaseState::handleToken(Token current) {
    if (current.getType() == TOKEN_TYPE_MESSAGE) {
        changeState(new ParserMessageState(parser));
    }
    else {
        throw ParseError(current);
    }
}

bool ParserBaseState::isTerminalState() {
    return true;
}

ParserMessageState::ParserMessageState(Parser* parser) {
    this->parser = parser;
}

void ParserMessageState::handleToken(Token current) {
    if (current.getType() == TOKEN_TYPE_IDENTIFIER) {
        getMessageBuilder()->setName(current.getValue());
        if (containsCustomType(current.getValue())) {
            throw ParseError(current);
        }
        else {
            insertCustomType(current.getValue());
        }
        changeState(new ParserNameState(parser));
    }
    else {
        throw ParseError(current);
    }
}

ParserNameState::ParserNameState(Parser* parser) {
    this->parser = parser;
}

void ParserNameState::handleToken(Token current) {
    if (current.getType() == TOKEN_TYPE_OPENING_CURLY) {
        changeState(new ParserOpenMessageState(parser));
    }
    else {
        throw ParseError(current);
    }
}

ParserOpenMessageState::ParserOpenMessageState(Parser* parser) {
    this->parser = parser;
}

void ParserOpenMessageState::handleToken(Token current) {
    if (current.getType() == TOKEN_TYPE_INT32
        || current.getType() == TOKEN_TYPE_STRING
        || current.getType() == TOKEN_TYPE_UINT32
        || current.getType() == TOKEN_TYPE_DOUBLE
        || current.getType() == TOKEN_TYPE_BYTES
        || current.getType() == TOKEN_TYPE_UINT64
        || current.getType() == TOKEN_TYPE_INT64
        || current.getType() == TOKEN_TYPE_BOOL
    ) {
        getFieldBuilder()->setTypeFromToken(current.getType());
        changeState(new ParserTypeState(parser));
    }
    else if (current.getType() == TOKEN_TYPE_CLOSING_CURLY) {
        try {
            addMessage(getMessageBuilder()->build());
            getMessageBuilder()->clear();
            changeState(new ParserBaseState(parser));
        } catch (MessageBuilderError e) {
            throw ParseError(e.getMessage());
        }
    }
    else if (current.getType() == TOKEN_TYPE_LIST) {
        getFieldBuilder()->setIsList(true);
    }
    else if (current.getType() == TOKEN_TYPE_IDENTIFIER && containsCustomType(current.getValue())) {
        getMessageBuilder()->addDependency(current.getValue());
        getFieldBuilder()->setCustomType(current.getValue());
        changeState(new ParserTypeState(parser));
    }
    else {
        throw ParseError(current);
    }
}

ParserListState::ParserListState(Parser* parser) {
    this->parser = parser;
}

void ParserListState::handleToken(Token current) {
    if (current.getType() == TOKEN_TYPE_INT32
        || current.getType() == TOKEN_TYPE_STRING
        || current.getType() == TOKEN_TYPE_UINT32
        || current.getType() == TOKEN_TYPE_DOUBLE
        || current.getType() == TOKEN_TYPE_BYTES
        || current.getType() == TOKEN_TYPE_UINT64
        || current.getType() == TOKEN_TYPE_INT64
        || current.getType() == TOKEN_TYPE_BOOL
    ) {
        getFieldBuilder()->setTypeFromToken(current.getType());
        changeState(new ParserTypeState(parser));
    }
    else if (current.getType() == TOKEN_TYPE_IDENTIFIER && containsCustomType(current.getValue())) {
        getMessageBuilder()->addDependency(current.getValue());
        getFieldBuilder()->setCustomType(current.getValue());
        changeState(new ParserTypeState(parser));
    }
    else {
        throw ParseError(current);
    }
}

ParserTypeState::ParserTypeState(Parser* parser) {
    this->parser = parser;
}

void ParserTypeState::handleToken(Token current) {
    if (current.getType() == TOKEN_TYPE_IDENTIFIER) {
        getFieldBuilder()->setName(current.getValue());
        changeState(new ParserFieldNameState(parser));
    }
    else {
        throw ParseError(current);
    }
}

ParserFieldNameState::ParserFieldNameState(Parser* parser) {
    this->parser = parser;
}

void ParserFieldNameState::handleToken(Token current) {
    if (current.getType() == TOKEN_TYPE_EQUALS) {
        changeState(new ParserEqualsState(parser));
    }
    else {
        throw ParseError(current);
    }
}

ParserEqualsState::ParserEqualsState(Parser* parser) {
    this->parser = parser;
}

void ParserEqualsState::handleToken(Token current) {
    if (current.getType() == TOKEN_TYPE_INDEX) {
        int index = stoi(current.getValue());

        if (index <= 0 || index > 8) {
            throw ParseError(current);
        }
        else {
            getFieldBuilder()->setIndex(index);
            changeState(new ParserIndexState(parser));
        }
    }
    else {
        throw ParseError(current);
    }
}

ParserIndexState::ParserIndexState(Parser* parser) {
    this->parser = parser;
}

void ParserIndexState::handleToken(Token current) {
    if (current.getType() == TOKEN_TYPE_SEMI_COLON) {
        try {
            getMessageBuilder()->addField(getFieldBuilder()->build());
            getFieldBuilder()->clear();
            changeState(new ParserOpenMessageState(parser));
        } catch (MessageFieldBuilderError e) {
            throw ParseError(e.getMessage());
        }
    }
    else {
        throw ParseError(current);
    }
}

