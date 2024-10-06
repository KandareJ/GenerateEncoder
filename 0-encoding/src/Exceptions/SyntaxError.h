#pragma once

#include <string>
#include <sstream>

#include "Error.h"

using namespace std;

class SyntaxError : public Error {
    public:
        SyntaxError(int line, char character);
};