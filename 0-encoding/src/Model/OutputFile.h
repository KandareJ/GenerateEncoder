#pragma once

#include <string>

class OutputFile {
    public:
        OutputFile(std::string path, std::string contents);
        std::string getPath();
        std::string getContents();
    private:
        std::string path;
        std::string contents;
};