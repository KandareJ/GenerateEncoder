#include "CppCodeGenerator.h"

std::vector<OutputFile> CppCodeGenerator::generateCode(std::unordered_map<std::string, Message> messages) {
    std::vector<OutputFile> outputs;
    outputs.push_back(OutputFile("JsonUtils.h", generateUtilHeader()));
    outputs.push_back(OutputFile("JsonUtils.cpp", generateUtilCpp()));

    for (unordered_map<std::string, Message>::iterator it = messages.begin(); it != messages.end(); it++) {
        std::string fileName = StringUtils::capitalize(it->first);
        outputs.push_back(OutputFile(fileName + ".h", generateHeader(it->second)));
        outputs.push_back(OutputFile(fileName + ".cpp", generateCpp(it->second)));
    }

    return outputs;
}

unordered_map<FieldType, std::string> CppCodeGenerator::typeMap = {
        {FIELD_TYPE_STRING, "std::string"},
        {FIELD_TYPE_INT32, "int"},
        {FIELD_TYPE_UINT32, "unsigned int"},
        {FIELD_TYPE_DOUBLE, "double"},
        {FIELD_TYPE_BYTES, "std::string"},
        {FIELD_TYPE_BOOL, "bool"},
        {FIELD_TYPE_UINT64, "unsigned long"},
        {FIELD_TYPE_INT64, "long"}
    };


string CppCodeGenerator::getTypeFromField(MessageField field) {
    if (field.getType() == FIELD_TYPE_CUSTOM) {
        return field.getCustomType();
    }
    else {
        return typeMap.at(field.getType());
    }
}

std::string CppCodeGenerator::generateHeader(Message message) {
    ostringstream os;

    os << "#pragma once" << endl << endl;
    os << "#include <sstream>" << endl;
    os << "#include <string>" << endl;
    os << "#include <vector>" << endl << endl;
    os << getIncludeHeaders();

    for (std::unordered_set<std::string>::iterator it = message.getDependencies().begin(); it != message.getDependencies().end(); it++) {
        os << "#include \"" << *it << ".h\"" << std::endl << std::endl;
    }

    os << generateClassHeader(message) << endl;
    os << generateClassBuilderHeader(message) << endl;
    

    return os.str();
}

std::string CppCodeGenerator::generateClassHeader(Message message) {
    ostringstream os;

    os << "class " << StringUtils::capitalize(message.getName()) << " {" << endl;
    os << "\tpublic:" << endl;
    os << "\t\tstd::string encode();" << endl;
    os << "\t\tvoid decode(std::string message);" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "\t\tstd::vector<" << getTypeFromField(message.getFields().at(i)) << ">";
        }
        else {
            os << "\t\t" << getTypeFromField(message.getFields().at(i));
        }
        os << " get" << StringUtils::capitalize(message.getFields().at(i).getName()) << "();" << endl;
    }

    os << "\tprivate:" << endl;
    os << "\t\tfriend class " << StringUtils::capitalize(message.getName()) << "Builder;" << endl;
    os << "\t\t"<< StringUtils::capitalize(message.getName()) << "(";

    for (int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "std::vector<" << getTypeFromField(message.getFields().at(i)) << ">";
        }
        else {
            os << getTypeFromField(message.getFields().at(i));
        }
        os << " " << message.getFields().at(i).getName();
        
        if (i < message.getFields().size() - 1) {
            os << ", ";
        }
    }

    os << ");" << endl;
    
    for (int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "\t\tstd::vector<" << getTypeFromField(message.getFields().at(i)) << ">";
        }
        else {
            os << "\t\t" << getTypeFromField(message.getFields().at(i));
        }
        os << " " << message.getFields().at(i).getName() << ";" << endl;
    }

    os << "};" << endl;

    return os.str();
}

std::string CppCodeGenerator::generateClassBuilderHeader(Message message) {
    ostringstream os;
    std::string className = StringUtils::capitalize(message.getName()) + "Builder";

    os << "class " << className << " {" << endl;
    os << "\tpublic:" << endl;
    os << "\t\t" << StringUtils::capitalize(message.getName()) << " build();" << endl;
    os << "\t\tvoid clear();" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "\t\t" << className << "* append";
        }
        else {
            os << "\t\t" << className << "* set";
        }

        os << StringUtils::capitalize(message.getFields().at(i).getName()) << "(";
        os << getTypeFromField(message.getFields().at(i)) << " ";
        os << message.getFields().at(i).getName() << ");" << endl;
    }

    os << "\tprivate:" << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "\t\tstd::vector<" << getTypeFromField(message.getFields().at(i)) << ">";
        }
        else {
            os << "\t\t" << getTypeFromField(message.getFields().at(i));
        }
        os << " " << message.getFields().at(i).getName() << ";" << endl;
    }

    os << "};" << endl;

    return os.str();
}

std::string CppCodeGenerator::generateCpp(Message message) {
    ostringstream os;
    std::string className = StringUtils::capitalize(message.getName());

    os << "#include \"" << message.getName() <<".h\"" << endl << endl;
    os << generateConstructor(message) << endl;
    os << generateEncode(message) << endl;
    os << generateDecode(message) << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << generateGetter(message.getFields().at(i), className) << endl;
    }

    os << generateBuild(message) << endl;

    for (int i = 0; i < message.getFields().size(); i++) {
        os << generateBuilderSetter(message.getFields().at(i), className) << endl;
    }

    os << generateClear(message) << endl;

    return os.str();
}

std::string CppCodeGenerator::generateConstructor(Message message) {
    ostringstream os;

    os << StringUtils::capitalize(message.getName()) << "::";
    os << StringUtils::capitalize(message.getName()) << "(";

    for (int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "std::vector<" << getTypeFromField(message.getFields().at(i)) << ">";

        }
        else {
            os << getTypeFromField(message.getFields().at(i));
        }
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

std::string CppCodeGenerator::generateGetter(MessageField field, std::string className) {
    ostringstream os;

    if (field.isList()) {
        os << "std::vector<" << getTypeFromField(field) << ">";

    }
    else {
        os << getTypeFromField(field);
    }
    
    os << " " << className << "::";
    os << "get" << StringUtils::capitalize(field.getName()) << "() {" << endl;
    os << "\treturn " << field.getName() << ";" << endl;
    os << "}" << endl;

    return os.str();
}

std::string CppCodeGenerator::generateBuilderSetter(MessageField field, std::string className) {
    ostringstream os;
    std::string builderClassName = className + "Builder";

    if (field.isList()) {
        os << builderClassName << "* " << builderClassName << "::";
        os << "append" << StringUtils::capitalize(field.getName()) << "(";
        os << getTypeFromField(field) << " " << field.getName() << ") {" << endl;
        os << "\tthis->" << field.getName() << ".push_back(" << field.getName() << ");" << endl;
        os << "\treturn this;" << endl;
        os << "}" << endl;
    }
    else {
        os << builderClassName << "* " << builderClassName << "::";
        os << "set" << StringUtils::capitalize(field.getName()) << "(";
        os << getTypeFromField(field) << " " << field.getName() << ") {" << endl;
        os << "\tthis->" << field.getName() << " = " << field.getName() << ";" << endl;
        os << "\treturn this;" << endl;
        os << "}" << endl;

    }
    
    return os.str();
}

std::string CppCodeGenerator::generateClear(Message message) {
    ostringstream os;
    std::string className = StringUtils::capitalize(message.getName()) + "Builder";

    os << "void " << className << "::clear() {" << endl;
    
    for (int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "\tthis->" << message.getFields().at(i).getName() << ".clear();" << endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_STRING
            || message.getFields().at(i).getType() == FIELD_TYPE_BYTES) {
            os << "\tthis->" << message.getFields().at(i).getName() << " = \"\";" << endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_INT32
            || message.getFields().at(i).getType() == FIELD_TYPE_UINT32
            || message.getFields().at(i).getType() == FIELD_TYPE_UINT64
            || message.getFields().at(i).getType() == FIELD_TYPE_INT64) {
            os << "\tthis->" << message.getFields().at(i).getName() << " = 0;" << endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_DOUBLE) {
            os << "\tthis->" << message.getFields().at(i).getName() << " = 0.0;" << endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_BOOL) {
            os << "\tthis->" << message.getFields().at(i).getName() << " = false;" << endl;
        }
    }

    os << "}" << endl;

    return os.str();
}

std::string CppCodeGenerator::generateBuild(Message message) {
    ostringstream os;
    std::string className = StringUtils::capitalize(message.getName());

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

