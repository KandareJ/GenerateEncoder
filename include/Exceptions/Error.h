#pragma once

#include <string>

class Error {
    public:
        Error();
        std::string getMessage();
        void setMessage(std::string message);
    private:
        std::string message;
};