#include "Utils/FileUtil.h"

std::string FileUtil::readFile(std::string filePath) {
    std::ostringstream os;
    std::ifstream file;
    file.open(filePath);

    if (file.fail()) {
        throw FileError("Unable to open file: " + filePath);
    }

    char current;

    while (file.get(current)) {
        os << current;
    }

    file.close();
    return os.str();
}

void FileUtil::writeFile(std::string filePath, std::string contents) {
    std::ofstream file;
    file.open(filePath);

    if (file.fail()) {
        throw FileError("Unable to open file: " + filePath);
    }

    file << contents;
    file.close();
}