#include "Exceptions/CliError.h"

CliError::CliError(std::string message) {
    setMessage(message);
}