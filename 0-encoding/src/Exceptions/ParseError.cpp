#include "ParseError.h"

ParseError::ParseError(Token token) {
    ostringstream os;
    os << "Parse Error: unexpected token \"" << token.getValue();
    os << "\" on line " << token.getLine();
    this->setMessage(os.str());
}

ParseError::ParseError(string message) {
    this->setMessage("Parse Error: " + message);
}