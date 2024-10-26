#pragma once

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

class FileUtil {
    public:
        static string readFile(string filePath);
        static void writeFile(string filePath, string contents);
};