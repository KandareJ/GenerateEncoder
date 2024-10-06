#pragma once

#include <sstream>

#include "Error.h"

class MessageFieldBuilderError : public Error {
    public:
        MessageFieldBuilderError(int index, string name);
};

class MessageBuilderError : public Error {
    public:
        MessageBuilderError(string name, string message);
        MessageBuilderError();
};