#include "Exceptions/FileError.h"

FileError::FileError(std::string message) {
    setMessage(message);
}
