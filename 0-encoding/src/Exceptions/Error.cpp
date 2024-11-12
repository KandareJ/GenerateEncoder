#include "Error.h"

Error::Error() {
    this->message = "";
}

std::string Error::getMessage() {
    return this->message;
}

void Error::setMessage(std::string message) {
    this->message = message;
}