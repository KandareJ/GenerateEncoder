#include "StringUtils.h"

string StringUtils::capitalize(string str) {
    if (!str.empty()) {
        str[0] = toupper(str[0]);
    }

    return str;
}