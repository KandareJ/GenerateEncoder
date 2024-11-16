#pragma once

#include <sstream>

#include "Exceptions/Error.h"

class MessageFieldBuilderError : public Error {
    public:
        MessageFieldBuilderError(int index, std::string name);
};

class MessageBuilderError : public Error {
    public:
        MessageBuilderError(std::string name, std::string message);
        MessageBuilderError();
};