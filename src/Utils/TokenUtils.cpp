#include "Utils/TokenUtils.h"

bool TokenUtils::isDigit(char character) {
    return character >= '0' && character <= '9';
}

bool TokenUtils::isCharacter(char character) {
    return (character >= 'A' && character <= 'Z')
        || (character >= 'a' && character <= 'z');
}