#pragma once

#include "Error.h"

class CliError : public Error {
    public:
        CliError(string message);
};