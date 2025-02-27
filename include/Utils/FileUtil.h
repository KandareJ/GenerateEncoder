#pragma once

#include <fstream>
#include <sstream>
#include <string>

#include "Exceptions/FileError.h"

class FileUtil {
    public:
        static std::string readFile(std::string filePath);
        static void writeFile(std::string filePath, std::string contents);
};