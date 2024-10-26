#pragma once

#include <string>

class StringUtils {
    public:
        static std::string capitalize(std::string str);
        static std::string createPath(std::string path, std::string name);
};