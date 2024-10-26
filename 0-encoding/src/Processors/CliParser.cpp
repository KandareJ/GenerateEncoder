#include "CliParser.h"

Command CliParser::parseCommand(int argc, char** argv) {
    if (argc != 5) {
        throw CliError("Unexpected number of arguments: " + getUsage());
    }

    string encodingType = string(argv[1]);
    string outputLanguage = string(argv[2]);
    string inputFile = string(argv[3]);
    string outputDirectory = string(argv[4]);
    
    if (encodingType != "json" && encodingType != "jbuf") {
        throw CliError("Unexpected encoding_type: " + getUsage());
    }

    if (outputLanguage != "cpp") {
        throw CliError("Unexpected output_language: " + getUsage());
    }

    return Command(encodingType, outputLanguage, inputFile, outputDirectory);
}

string CliParser::getUsage() {
    ostringstream os;
    os << "usage: GenerateEncoder encoding_type output_language input_file output_directory" << endl;
    os << "  options:" << endl;
    os << "    encoding_type\tjson, jbuf" << endl;
    os << "    output_langauge\tcpp" << endl;
    return os.str();
}