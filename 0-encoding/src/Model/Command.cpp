#include "Command.h"

Command::Command(std::string encodingType, std::string outputLanguage, std::string inputFile, std::string outputDirectory) {
    this->encodingType = encodingType;
    this->outputLanguage = outputLanguage;
    this->inputFile = inputFile;
    this->outputDirectory = outputDirectory;
}

std::string Command::getEncodingType() {
    return encodingType;
}

std::string Command::getOutputLanguage() {
    return outputLanguage;
}

std::string Command::getInputFile() {
    return inputFile;
}

std::string Command::getOutputDirectory() {
    return outputDirectory;
}