#pragma once

#include <sstream>

#include "Model/Command.h"
#include "Exceptions/CliError.h"

class CliParser {
    public:
        Command parseCommand(int argc, char** argv);
    private:
        std::string getUsage();
};