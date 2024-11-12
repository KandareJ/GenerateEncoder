#include "ParseError.h"

ParseError::ParseError(Token token) {
    std::ostringstream os;
    os << "Parse Error: unexpected token \"" << token.getValue();
    os << "\" on line " << token.getLine();
    this->setMessage(os.str());
}

ParseError::ParseError(std::string message) {
    this->setMessage("Parse Error: " + message);
}