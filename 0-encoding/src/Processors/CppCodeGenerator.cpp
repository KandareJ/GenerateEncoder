#include "CppCodeGenerator.h"

std::vector<OutputFile> CppCodeGenerator::generateCode(std::unordered_map<std::string, Message> messages) {
    std::vector<OutputFile> outputs;
    outputs.push_back(OutputFile("JsonUtils.h", generateUtilHeader()));
    outputs.push_back(OutputFile("JsonUtils.cpp", generateUtilCpp()));

    for (unordered_map<string, Message>::iterator it = messages.begin(); it != messages.end(); it++) {
        std::string fileName = StringUtils::capitalize(it->first);
        outputs.push_back(OutputFile(fileName + ".h", generateHeader(it->second)));
        outputs.push_back(OutputFile(fileName + ".cpp", generateCpp(it->second)));
    }

    return outputs;
}

unordered_map<FieldType, string> CppCodeGenerator::typeMap = {
        {FIELD_TYPE_STRING, "string"},
        {FIELD_TYPE_INT32, "int"},
        {FIELD_TYPE_UINT32, "unsigned int"},
        {FIELD_TYPE_DOUBLE, "double"},
        {FIELD_TYPE_BYTES, "string"},
        {FIELD_TYPE_BOOL, "bool"},
        {FIELD_TYPE_UINT64, "unsigned long"},
        {FIELD_TYPE_INT64, "long"}
    };

string CppCodeGenerator::generateHeader(Message message) {
    ostringstream os;

    os << "#pragma once" << endl << endl;
    os << "#include <sstream>" << endl;
    os << "#include <string>" << endl << endl;
    os << getIncludeHeaders() << endl;
    os << "using namespace std;" << endl << endl;
    os << generateClassHeader(message) << endl;
    os << generateClassBuilderHeader(message) << endl;
    

    return os.str();
}

string CppCodeGenerator::generateClassHeader(Message message) {
    ostringstream os;

    os << "class " << StringUtils::capitalize(message.getName()) << " {" << endl;
    os << "\tpublic:" << endl;
    os << "\t\tstring encode();" << endl;
    os << "\t\tvoid decode(string message);" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << "\t\t" << typeMap.at(message.getFields().at(i).getType());
        os << " get" << StringUtils::capitalize(message.getFields().at(i).getName()) << "();" << endl;
    }

    os << "\tprivate:" << endl;
    os << "\t\tfriend class " << StringUtils::capitalize(message.getName()) << "Builder;" << endl;
    os << "\t\t"<< StringUtils::capitalize(message.getName()) << "(";

    for (int i = 0; i < message.getFields().size(); i++) {
        os << typeMap.at(message.getFields().at(i).getType());
        os << " " << message.getFields().at(i).getName();
        
        if (i < message.getFields().size() - 1) {
            os << ", ";
        }
    }

    os << ");" << endl;
    
    for (int i = 0; i < message.getFields().size(); i++) {
        os << "\t\t" << typeMap.at(message.getFields().at(i).getType());
        os << " " << message.getFields().at(i).getName() << ";" << endl;
    }

    os << "};" << endl;

    return os.str();
}

string CppCodeGenerator::generateClassBuilderHeader(Message message) {
    ostringstream os;
    string className = StringUtils::capitalize(message.getName()) + "Builder";

    os << "class " << className << " {" << endl;
    os << "\tpublic:" << endl;
    os << "\t\t" << StringUtils::capitalize(message.getName()) << " build();" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << "\t\t" << className << "* set";
        os << StringUtils::capitalize(message.getFields().at(i).getName()) << "(";
        os << typeMap.at(message.getFields().at(i).getType()) << " ";
        os << message.getFields().at(i).getName() << ");" << endl;
    }

    os << "\tprivate:" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << "\t\t" << typeMap.at(message.getFields().at(i).getType());
        os << " " << message.getFields().at(i).getName() << ";" << endl;
    }

    os << "};" << endl;

    return os.str();
}

string CppCodeGenerator::generateCpp(Message message) {
    ostringstream os;
    string className = StringUtils::capitalize(message.getName());

    os << "#include \"" << message.getName() <<".h\"" << endl << endl;
    os << generateConstructor(message) << endl;
    os << generateEncode(message) << endl;
    os << generateDecode(message) << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << generateGetter(message.getFields().at(i), className) << endl;
    }

    os << generateBuild(message) << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << generateBuilderSetters(message.getFields().at(i), className) << endl;
    }

    return os.str();
}

string CppCodeGenerator::generateConstructor(Message message) {
    ostringstream os;

    os << StringUtils::capitalize(message.getName()) << "::";
    os << StringUtils::capitalize(message.getName()) << "(";

    for (int i = 0; i < message.getFields().size(); i++) {
        os << typeMap.at(message.getFields().at(i).getType());
        os << " " << message.getFields().at(i).getName();
        
        if (i < message.getFields().size() - 1) {
            os << ", ";
        }
    }

    os << ") {" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << "\tthis->" << message.getFields().at(i).getName();
        os << " = " << message.getFields().at(i).getName() << ";" << endl;
    }

    os << "}" << endl;

    return os.str();
}

string CppCodeGenerator::generateGetter(MessageField field, string className) {
    ostringstream os;
    
    os << typeMap.at(field.getType()) << " " << className << "::";
    os << "get" << StringUtils::capitalize(field.getName()) << "() {" << endl;
    os << "\treturn " << field.getName() << ";" << endl;
    os << "}" << endl;

    return os.str();
}

string CppCodeGenerator::generateBuilderSetters(MessageField field, string className) {
    ostringstream os;
    string builderClassName = className + "Builder";
    
    os << builderClassName << "* " << builderClassName << "::";
    os << "set" << StringUtils::capitalize(field.getName()) << "(";
    os << typeMap.at(field.getType()) << " " << field.getName() << ") {" << endl;
    os << "\tthis->" << field.getName() << " = " << field.getName() << ";" << endl;
    os << "\treturn this;" << endl;
    os << "}" << endl;

    return os.str();
}

string CppCodeGenerator::generateBuild(Message message) {
    ostringstream os;
    string className = StringUtils::capitalize(message.getName());

    os << className << " " << className << "Builder::build() {" << endl;
    os << "\treturn " << className << "(";

    for (int i = 0; i < message.getFields().size(); i++) {
        os << message.getFields().at(i).getName();
        if (i < message.getFields().size() - 1) {
            os << ", ";
        }
    }

    os << ");" << endl;
    os << "}" << endl;
    
    return os.str();
}

