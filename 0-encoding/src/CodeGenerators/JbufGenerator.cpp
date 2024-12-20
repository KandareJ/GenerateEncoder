#include "CodeGenerators/JbufGenerator.h"

std::string JbufGenerator::generateUtilHeader() {
    return "// TODO: write a JbufUtil class header";
}

std::string JbufGenerator::generateUtilCpp() {
    return "// TODO: write a JbufUtil class cpp file";
}

std::string JbufGenerator::getUtilFileName() {
    return "JbufUtils";
}

std::string JbufGenerator::getIncludeHeaders() {
    std::ostringstream os;
    os << "#include \"" << getUtilFileName() << ".h\"" << std::endl;
    return os.str();
}

std::string JbufGenerator::generateEncode(Message message) {
    return "";
}

std::string JbufGenerator::generateDecode(Message message) {
    return "";
}