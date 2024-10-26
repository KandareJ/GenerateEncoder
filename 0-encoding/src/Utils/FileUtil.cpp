#include "FileUtil.h"

string FileUtil::readFile(string filePath) {
    ostringstream os;
    ifstream file;
    file.open(filePath);

    char current;

    while (file.get(current)) {
        os << current;
    }

    file.close();
    return os.str();
}

void FileUtil::writeFile(string filePath, string contents) {
    ofstream file;
    file.open(filePath);
    file << contents;
    file.close();
}