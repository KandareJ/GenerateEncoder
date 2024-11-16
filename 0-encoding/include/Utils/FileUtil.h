#pragma once

#include <fstream>
#include <sstream>
#include <string>

class FileUtil {
    public:
        static std::string readFile(std::string filePath);
        static void writeFile(std::string filePath, std::string contents);
};