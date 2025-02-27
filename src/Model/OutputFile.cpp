#include "Model/OutputFile.h"

OutputFile::OutputFile(std::string path, std::string contents) {
    this->path = path;
    this->contents = contents;
}

std::string OutputFile::getPath() {
    return path;
}

std::string OutputFile::getContents() {
    return contents;
}