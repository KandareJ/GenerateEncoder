#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <sstream>

#include "MessageField.h"

class Message {
    public:
        string toString();
        string getName();
        vector<MessageField> getFields();
    private:
        friend class MessageBuilder;
        Message(vector<MessageField> fields, string name);
        vector<MessageField> fields;
        string name;
};

class MessageBuilder {
    public:
        MessageBuilder();
        MessageBuilder* setName(string name);
        MessageBuilder* addField(MessageField field);
        MessageBuilder* clear();
        Message build();
    private:
        vector<MessageField> fields;
        string name;
};