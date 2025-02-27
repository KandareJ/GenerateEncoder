#pragma once

#include "Exceptions/Error.h"

class FileError : public Error {
    public:
        FileError(std::string message);
};