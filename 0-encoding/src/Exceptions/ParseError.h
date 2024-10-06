#pragma once

#include <string>
#include <sstream>
#include "Error.h"
#include "../Model/Token.h"

class ParseError : public Error {
    public:
        ParseError(Token token);
};