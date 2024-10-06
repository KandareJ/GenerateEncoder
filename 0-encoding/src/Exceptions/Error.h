#pragma once

#include <string>

using namespace std;

class Error {
    public:
        Error();
        string getMessage();
        void setMessage(string message);
    private:
        string message;
};