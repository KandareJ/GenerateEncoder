#include "BuilderError.h"

MessageFieldBuilderError::MessageFieldBuilderError(int index, string name) {
    ostringstream os;
    os << "MessageFieldBuilderError: ";

    if (index <= 0) {
        os << "invalid index " << index;
    }
    else if (name == "") {
        os << "invalid name " << name;
    }
    else {
        os << "type not set";
    }

    this->setMessage(os.str());
}

MessageBuilderError::MessageBuilderError(string name, string message) {
    ostringstream os;
    os << "MessageBuidlerError for message " << name << ": " << message;
    this->setMessage(os.str());
}

MessageBuilderError::MessageBuilderError() {
    setMessage("MessageBuilderError: Missing name");
};