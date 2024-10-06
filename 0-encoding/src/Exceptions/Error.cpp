#include "Error.h"

Error::Error() {
    this->message = "";
}

string Error::getMessage() {
    return this->message;
}

void Error::setMessage(string message) {
    this->message = message;
}