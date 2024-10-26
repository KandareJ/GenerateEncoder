#pragma once

#include <string>

using namespace std;

class Command {
    public:
        Command(string encodingType, string outputLanguage, string inputFile, string outputDirectory);
        string getEncodingType();
        string getOutputLanguage();
        string getInputFile();
        string getOutputDirectory();
    private:
        string encodingType;
        string outputLanguage;
        string inputFile;
        string outputDirectory;
};