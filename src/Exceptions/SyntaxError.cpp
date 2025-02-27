#include "Exceptions/SyntaxError.h"

SyntaxError::SyntaxError(int line, char character) {
    std::ostringstream os;
    os << "Syntax Error on line " << line << ": " << character;
    this->setMessage(os.str());
}
