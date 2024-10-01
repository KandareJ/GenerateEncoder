#include "SyntaxError.h"

SyntaxError::SyntaxError(int line, char character) {
    this->line = line;
    this->character = character;
}

SyntaxError::~SyntaxError() {

}

string SyntaxError::getMessage() {
    ostringstream os;
    os << "Syntax Error on line " << line << ": " << character;
    return os.str();
}