#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "../Model/Token.h"
#include "../Model/Message.h"
#include "../Exceptions/ParseError.h"

class ParserState;

class Parser {
    public:
        Parser();
        ~Parser();
        unordered_map<string, Message> parse(vector<Token>);
    private:
        friend class ParserState;
        void changeState(ParserState* newState);
        unordered_map<string, Message> messages;
        unordered_set<string> customTypes;
        ParserState* state;
        MessageFieldBuilder* messageFieldBuilder;
        MessageBuilder* messageBuilder;
};

class ParserState {
    public:
        virtual void handleToken(Token current) = 0;
        virtual bool isTerminalState();
    protected:
        void changeState(ParserState* state);
        MessageFieldBuilder* getFieldBuilder();
        MessageBuilder* getMessageBuilder();
        void addMessage(Message message);
        void insertCustomType(string type);
        bool containsCustomType(string type);
        Parser* parser;
};

class ParserBaseState : public ParserState {
    public:
        ParserBaseState(Parser* parser);
        virtual void handleToken(Token current);
        virtual bool isTerminalState();
};

class ParserMessageState : public ParserState {
    public:
        ParserMessageState(Parser* parser);
        virtual void handleToken(Token current);
};

class ParserNameState : public ParserState {
    public:
        ParserNameState(Parser* parser);
        virtual void handleToken(Token current);
};

class ParserOpenMessageState : public ParserState {
    public:
        ParserOpenMessageState(Parser* parser);
        virtual void handleToken(Token current);
};

class ParserTypeState : public ParserState {
    public:
        ParserTypeState(Parser* parser);
        virtual void handleToken(Token current);
};

class ParserFieldNameState : public ParserState {
    public:
        ParserFieldNameState(Parser* parser);
        virtual void handleToken(Token current);
};

class ParserEqualsState : public ParserState {
    public:
        ParserEqualsState(Parser* parser);
        virtual void handleToken(Token current);
};

class ParserIndexState : public ParserState {
    public:
        ParserIndexState(Parser* parser);
        virtual void handleToken(Token current);
};

class ParserListState : public ParserState {
    public:
        ParserListState(Parser* parser);
        virtual void handleToken(Token current);
};
