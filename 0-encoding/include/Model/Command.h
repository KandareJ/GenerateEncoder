#pragma once

#include <string>

class Command {
    public:
        Command(std::string encodingType, std::string outputLanguage, std::string inputFile, std::string outputDirectory);
        std::string getEncodingType();
        std::string getOutputLanguage();
        std::string getInputFile();
        std::string getOutputDirectory();
    private:
        std::string encodingType;
        std::string outputLanguage;
        std::string inputFile;
        std::string outputDirectory;
};