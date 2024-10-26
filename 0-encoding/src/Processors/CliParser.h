#pragma once

#include <sstream>

#include "../Model/Command.h"
#include "../Exceptions/CliError.h"

using namespace std;

class CliParser {
    public:
        Command parseCommand(int argc, char** argv);
    private:
        string getUsage();
};