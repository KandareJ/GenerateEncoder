#pragma once

#include <string>
#include <sstream>

#include "Exceptions/Error.h"

class SyntaxError : public Error {
    public:
        SyntaxError(int line, char character);
};