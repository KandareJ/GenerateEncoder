#include "FileUtil.h"

std::string FileUtil::readFile(std::string filePath) {
    std::ostringstream os;
    std::ifstream file;
    file.open(filePath);

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
    file << contents;
    file.close();
}