#include "CodeGenerators/CppCodeGenerator.h"

std::vector<OutputFile> CppCodeGenerator::generateCode(std::unordered_map<std::string, Message> messages) {
    std::vector<OutputFile> outputs;
    outputs.push_back(OutputFile(getUtilFileName() + ".h", generateUtilHeader()));
    outputs.push_back(OutputFile(getUtilFileName() + ".cpp", generateUtilCpp()));

    for (std::unordered_map<std::string, Message>::iterator it = messages.begin(); it != messages.end(); it++) {
        std::string fileName = StringUtils::capitalize(it->first);
        outputs.push_back(OutputFile(fileName + ".h", generateHeader(it->second)));
        outputs.push_back(OutputFile(fileName + ".cpp", generateCpp(it->second)));
    }

    return outputs;
}

std::unordered_map<FieldType, std::string> CppCodeGenerator::typeMap = {
        {FIELD_TYPE_STRING, "std::string"},
        {FIELD_TYPE_INT32, "int"},
        {FIELD_TYPE_UINT32, "unsigned int"},
        {FIELD_TYPE_DOUBLE, "double"},
        {FIELD_TYPE_BYTES, "std::string"},
        {FIELD_TYPE_BOOL, "bool"},
        {FIELD_TYPE_UINT64, "unsigned long"},
        {FIELD_TYPE_INT64, "long"}
    };


std::string CppCodeGenerator::getTypeFromField(MessageField field) {
    if (field.getType() == FIELD_TYPE_CUSTOM) {
        return field.getCustomType();
    }
    else {
        return typeMap.at(field.getType());
    }
}

std::string CppCodeGenerator::generateHeader(Message message) {
    std::ostringstream os;
    std::unordered_set<std::string> dependencies = message.getDependencies();

    os << "#pragma once" << std::endl << std::endl;
    os << "#include <sstream>" << std::endl;
    os << "#include <string>" << std::endl;
    os << "#include <vector>" << std::endl << std::endl;
    os << getIncludeHeaders();

    for (std::unordered_set<std::string>::iterator it = dependencies.begin(); it != dependencies.end(); it++) {
        os << "#include \"" << *it << ".h\"" << std::endl << std::endl;
    }

    os << generateClassHeader(message) << std::endl;
    os << generateClassBuilderHeader(message) << std::endl;
    
    return os.str();
}

std::string CppCodeGenerator::generateAdditionalPublic(Message message) {
    return "";
}

std::string CppCodeGenerator::generateAdditionalPrivate(Message message) {
    return "";
}

std::string CppCodeGenerator::generateClassHeader(Message message) {
    std::ostringstream os;

    os << "class " << StringUtils::capitalize(message.getName()) << " {" << std::endl;
    os << "\tpublic:" << std::endl;
    os << "\t\tstd::string encode();" << std::endl;
    os << "\t\tvoid decode(std::string message);" << std::endl;

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "\t\tstd::vector<" << getTypeFromField(message.getFields().at(i)) << ">";
        }
        else {
            os << "\t\t" << getTypeFromField(message.getFields().at(i));
        }
        os << " get" << StringUtils::capitalize(message.getFields().at(i).getName()) << "();" << std::endl;
    }

    os << generateAdditionalPublic(message);

    os << "\tprivate:" << std::endl;
    os << "\t\tfriend class " << StringUtils::capitalize(message.getName()) << "Builder;" << std::endl;
    os << "\t\t"<< StringUtils::capitalize(message.getName()) << "(";

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
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

    os << ");" << std::endl;
    
    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "\t\tstd::vector<" << getTypeFromField(message.getFields().at(i)) << ">";
        }
        else {
            os << "\t\t" << getTypeFromField(message.getFields().at(i));
        }
        os << " " << message.getFields().at(i).getName() << ";" << std::endl;
    }

    os << generateAdditionalPrivate(message);

    os << "};" << std::endl;

    return os.str();
}

std::string CppCodeGenerator::generateClassBuilderConstructor(Message message) {
    std::ostringstream os;

    if (message.getDependencies().size()) {
        std::string className = StringUtils::capitalize(message.getName());
        os << className << "Builder::" << className << "Builder()";

        std::vector<MessageField> customFields;
        
        for (unsigned int i = 0; i < message.getFields().size(); i++) {
            if (message.getFields().at(i).getType() == FIELD_TYPE_CUSTOM) {
                customFields.push_back(message.getFields().at(i));
            }
        }

        for (unsigned int i = 0; i < customFields.size(); i++) {
            if (i == 0) {
                os << " : ";
            }
            os << customFields.at(i).getName() << "(" << customFields.at(i).getCustomType() << "Builder::empty())";
            if (i < customFields.size() -1) {
                os << ", ";
            }
        }

        os << " { }" << std::endl;
    }

    return os.str();
}

std::string CppCodeGenerator::generateClassBuilderHeader(Message message) {
    std::ostringstream os;
    std::string className = StringUtils::capitalize(message.getName()) + "Builder";

    os << "class " << className << " {" << std::endl;
    os << "\tpublic:" << std::endl;
    if (message.getDependencies().size()) {
        os << "\t\t" << className << "();" << std::endl;
    }
    os << "\t\t" << StringUtils::capitalize(message.getName()) << " build();" << std::endl;
    os << "\t\tvoid clear();" << std::endl;

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "\t\t" << className << "* append";
        }
        else {
            os << "\t\t" << className << "* set";
        }

        os << StringUtils::capitalize(message.getFields().at(i).getName()) << "(";
        os << getTypeFromField(message.getFields().at(i)) << " ";
        os << message.getFields().at(i).getName() << ");" << std::endl;
    }

    os << "\t\tstatic " << StringUtils::capitalize(message.getName()) << " empty();" << std::endl;

    os << "\tprivate:" << std::endl;

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "\t\tstd::vector<" << getTypeFromField(message.getFields().at(i)) << ">";
        }
        else {
            os << "\t\t" << getTypeFromField(message.getFields().at(i));
        }
        os << " " << message.getFields().at(i).getName() << ";" << std::endl;
    }

    os << "};" << std::endl;

    return os.str();
}

std::string CppCodeGenerator::generateCpp(Message message) {
    std::ostringstream os;
    std::string className = StringUtils::capitalize(message.getName());

    os << "#include \"" << message.getName() <<".h\"" << std::endl << std::endl;
    os << generateConstructor(message) << std::endl;
    os << generateEncode(message) << std::endl;
    os << generateDecode(message) << std::endl;

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        os << generateGetter(message.getFields().at(i), className) << std::endl;
    }

    os << generateClassBuilderConstructor(message) << std::endl;

    os << generateBuild(message) << std::endl;

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        os << generateBuilderSetter(message.getFields().at(i), className) << std::endl;
    }

    os << generateClear(message) << std::endl;
    os << generateEmpty(message) << std::endl;

    return os.str();
}

std::string CppCodeGenerator::generateConstructor(Message message) {
    std::ostringstream os;

    os << StringUtils::capitalize(message.getName()) << "::";
    os << StringUtils::capitalize(message.getName()) << "(";

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
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

    os << ")";

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        if (i == 0) {
            os << " : ";
        }
        os << message.getFields().at(i).getName() << "(" << message.getFields().at(i).getName() << ")";
        if (i < message.getFields().size() - 1) {
            os << ", ";
        }
    }
    
    os << " { }" << std::endl;

    

    return os.str();
}

std::string CppCodeGenerator::generateGetter(MessageField field, std::string className) {
    std::ostringstream os;

    if (field.isList()) {
        os << "std::vector<" << getTypeFromField(field) << ">";

    }
    else {
        os << getTypeFromField(field);
    }
    
    os << " " << className << "::";
    os << "get" << StringUtils::capitalize(field.getName()) << "() {" << std::endl;
    os << "\treturn " << field.getName() << ";" << std::endl;
    os << "}" << std::endl;

    return os.str();
}

std::string CppCodeGenerator::generateBuilderSetter(MessageField field, std::string className) {
    std::ostringstream os;
    std::string builderClassName = className + "Builder";

    if (field.isList()) {
        os << builderClassName << "* " << builderClassName << "::";
        os << "append" << StringUtils::capitalize(field.getName()) << "(";
        os << getTypeFromField(field) << " " << field.getName() << ") {" << std::endl;
        os << "\tthis->" << field.getName() << ".push_back(" << field.getName() << ");" << std::endl;
        os << "\treturn this;" << std::endl;
        os << "}" << std::endl;
    }
    else {
        os << builderClassName << "* " << builderClassName << "::";
        os << "set" << StringUtils::capitalize(field.getName()) << "(";
        os << getTypeFromField(field) << " " << field.getName() << ") {" << std::endl;
        os << "\tthis->" << field.getName() << " = " << field.getName() << ";" << std::endl;
        os << "\treturn this;" << std::endl;
        os << "}" << std::endl;

    }
    
    return os.str();
}

std::string CppCodeGenerator::generateClear(Message message) {
    std::ostringstream os;
    std::string className = StringUtils::capitalize(message.getName()) + "Builder";

    os << "void " << className << "::clear() {" << std::endl;
    
    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).isList()) {
            os << "\tthis->" << message.getFields().at(i).getName() << ".clear();" << std::endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_STRING
            || message.getFields().at(i).getType() == FIELD_TYPE_BYTES) {
            os << "\tthis->" << message.getFields().at(i).getName() << " = \"\";" << std::endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_INT32
            || message.getFields().at(i).getType() == FIELD_TYPE_UINT32
            || message.getFields().at(i).getType() == FIELD_TYPE_UINT64
            || message.getFields().at(i).getType() == FIELD_TYPE_INT64) {
            os << "\tthis->" << message.getFields().at(i).getName() << " = 0;" << std::endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_DOUBLE) {
            os << "\tthis->" << message.getFields().at(i).getName() << " = 0.0;" << std::endl;
        }
        else if (message.getFields().at(i).getType() == FIELD_TYPE_BOOL) {
            os << "\tthis->" << message.getFields().at(i).getName() << " = false;" << std::endl;
        }
    }

    os << "}" << std::endl;

    return os.str();
}

std::string CppCodeGenerator::generateEmpty(Message message) {
    std::ostringstream os;
    std::string className = StringUtils::capitalize(message.getName());

    os << className << " " << className << "Builder::empty() {" << std::endl;
    os << "\t" << className << "Builder builder;" << std::endl;

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        if (message.getFields().at(i).getType() == FIELD_TYPE_CUSTOM) {
            os << "\tbuilder.set" << StringUtils::capitalize(message.getFields().at(i).getName()) << "("<< message.getFields().at(i).getCustomType() << "Builder::empty());" << std::endl;
        }
    }

    os << "\treturn builder.build();" << std::endl;
    os << "}" << std::endl;

    return os.str();
}

std::string CppCodeGenerator::generateBuild(Message message) {
    std::ostringstream os;
    std::string className = StringUtils::capitalize(message.getName());

    os << className << " " << className << "Builder::build() {" << std::endl;
    os << "\treturn " << className << "(";

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        os << message.getFields().at(i).getName();
        if (i < message.getFields().size() - 1) {
            os << ", ";
        }
    }

    os << ");" << std::endl;
    os << "}" << std::endl;
    
    return os.str();
}

