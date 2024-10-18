#include "BuilderError.h"

MessageFieldBuilderError::MessageFieldBuilderError(int index, string name) {
    ostringstream os;
    os << "Unable to set field " << name << ": ";
    
    if (index <= 0) {
        os << "Invalid index " << index;
    }
    else if (name == "") {
        os << "Invalid name " << name;
    }
    else {
        os << "Type not set";
    }

    this->setMessage(os.str());
}

MessageBuilderError::MessageBuilderError(string name, string message) {
    ostringstream os;
    os << "Unable to build message " << name << ": " << message;
    this->setMessage(os.str());
}

MessageBuilderError::MessageBuilderError() {
    setMessage("Attempted to build a message without a name set");
};