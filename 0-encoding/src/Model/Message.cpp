#include "Model/Message.h"

Message::Message(std::vector<MessageField> fields, std::string name, std::unordered_set<std::string> dependencies) {
    this->fields = fields;
    this->name = name;
    this->dependencies = dependencies;
}

std::string Message::toString() {
    std::ostringstream os;
    os << "<Message " << name << std::endl;
    for (int i = 0; i < fields.size(); i++) {
        os << "\t" << fields.at(i).toString() << std::endl;
    }
    os << ">";

    return os.str();
}

std::string Message::getName() {
    return name;
}

std::vector<MessageField> Message::getFields() {
    return fields;
}

std::unordered_set<std::string> Message::getDependencies() {
    return dependencies;
}

MessageBuilder::MessageBuilder() {
    this->clear();
}

MessageBuilder* MessageBuilder::setName(std::string name) {
    this->name = name;
    return this;
}

MessageBuilder* MessageBuilder::addField(MessageField field) {
    fields.push_back(field);
    return this;
}

MessageBuilder* MessageBuilder::addDependency(std::string dependency) {
    dependencies.insert(dependency);
    return this;
}

MessageBuilder* MessageBuilder::clear() {
    name = "";
    fields.clear();
    dependencies.clear();
    return this;
}

Message MessageBuilder::build() {
    if (name == "") {
        throw MessageBuilderError();
    }
    
    std::unordered_set<int> indexes;
    std::unordered_set<std::string> names;

    for (int i = 0; i < fields.size(); i++) {
        if (indexes.find(fields.at(i).getIndex()) == indexes.end()) {
            indexes.insert(fields.at(i).getIndex());
        }
        else {
            std::ostringstream os;
            os << "Duplicate index " << fields.at(i).getIndex();
            throw MessageBuilderError(name, os.str());
        }

        if (names.find(fields.at(i).getName()) == names.end()) {
            names.insert(fields.at(i).getName());
        }
        else {
            std::ostringstream os;
            os << "Duplicate name " << fields.at(i).getName();
            throw MessageBuilderError(name, os.str());
        }
    }

    return Message(fields, name, dependencies);
}