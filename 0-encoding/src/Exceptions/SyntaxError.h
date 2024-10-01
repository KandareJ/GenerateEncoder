#pragma once

#include <string>
#include <sstream>

using namespace std;

class SyntaxError {
    public:
        SyntaxError(int line, char character);
        ~SyntaxError();
        string getMessage();
    private:
        int line;
        char character;
};