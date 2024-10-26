#include "Command.h"

Command::Command(string encodingType, string outputLanguage, string inputFile, string outputDirectory) {
    this->encodingType = encodingType;
    this->outputLanguage = outputLanguage;
    this->inputFile = inputFile;
    this->outputDirectory = outputDirectory;
}

string Command::getEncodingType() {
    return encodingType;
}

string Command::getOutputLanguage() {
    return outputLanguage;
}

string Command::getInputFile() {
    return inputFile;
}

string Command::getOutputDirectory() {
    return outputDirectory;
}