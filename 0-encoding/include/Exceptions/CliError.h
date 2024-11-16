#pragma once

#include "Exceptions/Error.h"

class CliError : public Error {
    public:
        CliError(std::string message);
};