#include "StringUtils.h"

std::string StringUtils::capitalize(std::string str) {
    if (!str.empty()) {
        str[0] = toupper(str[0]);
    }

    return str;
}

std::string StringUtils::createPath(std::string path, std::string name) {
    if (path.length() && path.at(path.length() - 1) != '/') {
        return path + "/" + name;
    }
    else {
        return path + name;
    }
}