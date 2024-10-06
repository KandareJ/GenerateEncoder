#include "Message.h"

Message::Message(vector<MessageField> fields, string name) {
    this->fields = fields;
    this->name = name;
}

string Message::toString() {
    ostringstream os;
    os << "<Message " << name << endl;
    for (int i = 0; i < fields.size(); i++) {
        os << "\t" << fields.at(i).toString() << endl;
    }
    os << ">";

    return os.str();
}

MessageBuilder::MessageBuilder() {
    this->clear();
}

MessageBuilder* MessageBuilder::setName(string name) {
    this->name = name;
    return this;
}

MessageBuilder* MessageBuilder::addField(MessageField field) {
    fields.push_back(field);
    return this;
}

MessageBuilder* MessageBuilder::clear() {
    name = "";
    fields.clear();
    return this;
}

Message MessageBuilder::build() {
    if (name == "") {
        throw MessageBuilderError();
    }
    
    unordered_set<int> indexes;
    unordered_set<string> names;

    for (int i = 0; i < fields.size(); i++) {
        if (indexes.find(fields.at(i).getIndex()) != indexes.end()) {
            indexes.insert(fields.at(i).getIndex());
        }
        else {
            throw MessageBuilderError(name, "Duplicate index " + fields.at(i).getIndex());
        }

        if (names.find(fields.at(i).getName()) != names.end()) {
            names.insert(fields.at(i).getName());
        }
        else {
            throw MessageBuilderError(name, "Duplicate name " + fields.at(i).getName());
        }
    }

    return Message(fields, name);
}