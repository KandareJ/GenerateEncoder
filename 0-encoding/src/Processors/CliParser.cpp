#include "CliParser.h"

Command CliParser::parseCommand(int argc, char** argv) {
    if (argc != 5) {
        throw CliError("Unexpected number of arguments: " + getUsage());
    }

    std::string encodingType = std::string(argv[1]);
    std::string outputLanguage = std::string(argv[2]);
    std::string inputFile = std::string(argv[3]);
    std::string outputDirectory = std::string(argv[4]);
    
    if (encodingType != "json" && encodingType != "jbuf") {
        throw CliError("Unexpected encoding_type: " + getUsage());
    }

    if (outputLanguage != "cpp") {
        throw CliError("Unexpected output_language: " + getUsage());
    }

    return Command(encodingType, outputLanguage, inputFile, outputDirectory);
}

std::string CliParser::getUsage() {
    std::ostringstream os;
    os << "usage: GenerateEncoder encoding_type output_language input_file output_directory" << std::endl;
    os << "  options:" << std::endl;
    os << "    encoding_type\tjson, jbuf" << std::endl;
    os << "    output_langauge\tcpp" << std::endl;
    return os.str();
}