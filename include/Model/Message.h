#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <sstream>

#include "Model/MessageField.h"

class Message {
    public:
        std::string toString();
        std::string getName();
        std::vector<MessageField> getFields();
        std::unordered_set<std::string> getDependencies();
    private:
        friend class MessageBuilder;
        Message(std::vector<MessageField> fields, std::string name, std::unordered_set<std::string> dependencies);
        std::vector<MessageField> fields;
        std::unordered_set<std::string> dependencies;
        std::string name;
};

class MessageBuilder {
    public:
        MessageBuilder();
        MessageBuilder* setName(std::string name);
        MessageBuilder* addField(MessageField field);
        MessageBuilder* addDependency(std::string dependency);
        MessageBuilder* clear();
        Message build();
    private:
        std::vector<MessageField> fields;
        std::unordered_set<std::string> dependencies;
        std::string name;
};