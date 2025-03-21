#include "CodeGenerators/JbufGenerator.h"

std::string JbufGenerator::getUtilFileName() {
    return "JbufUtils";
}

std::string JbufGenerator::getIncludeHeaders() {
    std::ostringstream os;
    os << "#include \"" << getUtilFileName() << ".h\"" << std::endl;
    return os.str();
}

std::string JbufGenerator::generateEncode(Message message) {
    std::ostringstream os;

    os << "std::string " << StringUtils::capitalize(message.getName()) << "::encode() {" << std::endl;
    os << "\tstd::ostringstream os;" << std::endl << std::endl;

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        os << generateEncodeField(message.getFields().at(i)) << std::endl;
    }
    
    os << std::endl << "\treturn os.str();" << std::endl;
    os << "}" << std::endl;

    return os.str();
}

std::string JbufGenerator::generateEncodeField(MessageField field) {
    std::ostringstream os;

    if (field.isList()) {
        os << "\tfor (unsigned int i = 0; i < " << field.getName() << ".size(); i++) {" << std::endl;

        if (field.getType() == FIELD_TYPE_CUSTOM) {
           os << "\t\tos << JbufUtils::encode(" << field.getIndex() << ", ";
           os << field.getName() << ".at(i).encode());";
        }
        else {
            os << "\t\tos << JbufUtils::encode(" << field.getIndex() << ", ";
            os << field.getName() << ".at(i));" << std::endl;
        }
        
        os << "\t}";
    }
    else {
        if (field.getType() == FIELD_TYPE_CUSTOM) {
           os << "\tos << JbufUtils::encode(" << field.getIndex() << ", ";
           os << field.getName() << ".encode());";
        }
        else {
            os << "\tos << JbufUtils::encode(" << field.getIndex() << ", " <<  field.getName() << ");";
        }
    }

    return os.str();
}

std::string JbufGenerator::generateDecode(Message message) {
    std::ostringstream os;
    
    os << "void " << StringUtils::capitalize(message.getName()) << "::decode(std::string message) {" << std::endl;
    os << "\tJbufParser parser(message);" << std::endl;

    for (unsigned int i = 0; i < message.getFields().size(); i++) {
        MessageField field = message.getFields().at(i);

        if (field.isList()) {
            os << "\tthis->" << field.getName() << ".clear();" << std::endl;
            if (field.getType() == FIELD_TYPE_INT32) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToInt32List();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_INT64) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToInt64List();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_UINT32) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToUint32List();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_UINT64) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToUint64List();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_DOUBLE) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToDoubleList();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_BOOL) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToBoolList();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_STRING) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToStringList();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_BYTES) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToBytesList();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_CUSTOM) {
                os << "\tstd::vector<std::string> " << field.getName() << "BytesList = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToBytesList();" << std::endl;

                os << "\tfor (unsigned int i = 0; i < " << field.getName() << "BytesList.size(); i++) {" << std::endl;
                    os << "\t\t" << field.getCustomType() << " customType;" << std::endl;
                    os << "\t\tcustomType.decode(" << field.getName() << "BytesList.at(i));" << std::endl;
                    os << "\t\tthis->" << field.getName() << ".push_back(customType);" << std::endl;
                os << "\t}" << std::endl;
            }
        }
        else {
            if (field.getType() == FIELD_TYPE_INT32) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToInt32();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_INT64) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToInt64();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_UINT32) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToUint32();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_UINT64) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToUint64();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_DOUBLE) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToDouble();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_BOOL) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToBool();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_STRING) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToString();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_BYTES) {
                os << "\tthis->" << field.getName() << " = parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToBytes();" << std::endl;
            }
            else if (field.getType() == FIELD_TYPE_CUSTOM) {
                os << "\tthis->" << field.getName() << ".decode(parser.getJbufObject()";
                os << "->getValue(" << field.getIndex() << ")->decodeToBytes());" << std::endl;
            }
        }
    }

    os << "}" << std::endl;
    
    return os.str();
}

std::string JbufGenerator::generateUtilHeader() {
    return R"(#pragma once

#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>

class JbufUtils {
    public:
        static std::string encode(int tag, std::string value);
        static std::string encode(int tag, int value);
        static std::string encode(int tag, double value);
        static std::string encode(int tag, long value);
        static std::string encode(int tag, bool value);
        static std::string encode(int tag, unsigned int value);
        static std::string encode(int tag, unsigned long value);
        static bool isBigEndianSystem();
        static void reverseSubarray(char* array, int start, int end);
    private:
        static constexpr union {
            unsigned int i;
            char c[4];
        } endianTester = {0x01020304};
};

enum JBUF_DATA_SIZE {
    JBUF_DATA_SIZE_EMPTY, // defaults to this when value is not set
    JBUF_DATA_SIZE_ONE_BYTE,
    JBUF_DATA_SIZE_FOUR_BYTES,
    JBUF_DATA_SIZE_EIGHT_BYTES,
    JBUF_DATA_SIZE_VARIABLE
};

class JbufObject;

class JbufValue {
    public:
        int decodeToInt32();
        unsigned int decodeToUint32();
        double decodeToDouble();
        long decodeToInt64();
        unsigned long decodeToUint64();
        std::string decodeToString();
        std::string decodeToBytes();
        bool decodeToBool();
        std::vector<int> decodeToInt32List();
        std::vector<unsigned int> decodeToUint32List();
        std::vector<double> decodeToDoubleList();
        std::vector<long> decodeToInt64List();
        std::vector<unsigned long> decodeToUint64List();
        std::vector<std::string> decodeToStringList();
        std::vector<std::string> decodeToBytesList();
        std::vector<bool> decodeToBoolList();
    private:
        friend class JbufObject;
        JbufValue(JBUF_DATA_SIZE dataSize);
        JBUF_DATA_SIZE dataSize;
        std::vector<std::string> entries;
};

class JbufObject {
    public:
        ~JbufObject();
        JbufValue* getValue(int tag);
        void insertValue(int tag, JBUF_DATA_SIZE size, std::string value);
    private:
        std::unordered_map<int, JbufValue*> map;
};

class JbufError {
    public:
        JbufError(std::string);
        std::string getMessage();
    private:
        std::string message;
};

class JbufParser {
    public:
        JbufParser(std::string input);
        ~JbufParser();
        JbufObject* getJbufObject();
    private:
        int readTag();
        JBUF_DATA_SIZE readSizeType();
        unsigned long readDataSize(JBUF_DATA_SIZE sizeType);
        unsigned long readVariableDataSize();
        JbufObject* object;
        std::string input;
        unsigned long index;
};)";
}

std::string JbufGenerator::generateUtilCpp() {
    return R"(#include "JbufUtils.h"

const char VARIABLE_BYTE_SIZE = 0x00;
const char ONE_BYTE_SIZE = 0x40;
const char FOUR_BYTE_SIZE = 0x80;
const char EIGHT_BYTE_SIZE = 0xc0;

bool JbufUtils::isBigEndianSystem() {
    return endianTester.c[0] == 1;
}

void JbufUtils::reverseSubarray(char* array, int start, int end) {
    while (start < end - 1) {
        char temp = array[start];
        array[start] = array[end - 1];
        array[end - 1] = temp;
        start++;
        end--;
    }
}

/*
    2 bits for data size
    0 - variable size (string, bytes)
    1 - 1 byte (bool)
    2 - 4 bytes (uint32, int32)
    3 - 8 bytes (int64, uint64, double)
*/

std::string JbufUtils::encode(int tag, std::string value) {
    if (value == "") return "";
    std::string encodedBytes;

    char tagAndSize = VARIABLE_BYTE_SIZE;
    tagAndSize |= tag & 0x3f;
    encodedBytes.push_back(tagAndSize);
    
    std::size_t length = value.length();

    while (length > 127) {
        // cast length to char
        char lsb = (char) length;
        // set most significant bit to indicate more bits of size
        lsb |= 0x40;
        // push byte onto our encoded bytes
        encodedBytes.push_back(lsb);
        // right shift 7 bits that we've accounted for
        length = length >> 7;
    }

    // cast length to char
    char lsb = (char) length;
    // drop most significant bit to indicate end of size
    lsb &= 0x7f;
    // push byte onto our encoded bytes
    encodedBytes.push_back(lsb);

    // follow with the payload
    encodedBytes.append(value);

    return encodedBytes;
}

std::string JbufUtils::encode(int tag, int value) {
    if (value == 0) {
        return "";
    }
    // encoded size is 5 bytes - 1 byte for storing data size and tag number
    // and 4 bytes for the payload
    int encodedSize = 5;
    char* encodedBytes = new char[encodedSize];

    // set the data size in the top 2 bits
    encodedBytes[0] = FOUR_BYTE_SIZE;
    
    // set the tag in the bottom 6 bits of the first byte
    encodedBytes[0] |= tag & 0x3f;

    // copy the integer bytes into the last 4 bytes of the message
    std::memcpy(encodedBytes + 1, &value, sizeof(int));

    // We choose to store data in little endian. If arch is big endian, reverse the memory
    if (isBigEndianSystem()) {
        reverseSubarray(encodedBytes, 1, encodedSize);
    }

    std::string encodedString(encodedBytes, encodedSize);

    delete[] encodedBytes;
    return encodedString;
}

std::string JbufUtils::encode(int tag, double value) {
    if (value == 0.0) {
        return "";
    }

    int encodedSize = 9;
    char* encodedBytes = new char[encodedSize];

    encodedBytes[0] = EIGHT_BYTE_SIZE;
    encodedBytes[0] |= tag & 0x3f;
    std::memcpy(encodedBytes + 1, &value, sizeof(double));

    if (isBigEndianSystem()) {
        reverseSubarray(encodedBytes, 1, encodedSize);
    }

    std::string encodedString(encodedBytes, encodedSize);

    delete[] encodedBytes;
    return encodedString;
}

std::string JbufUtils::encode(int tag, long value) {
    if (value == 0) {
        return "";
    }

    int encodedSize = 9;
    char* encodedBytes = new char[encodedSize];

    encodedBytes[0] = EIGHT_BYTE_SIZE;
    encodedBytes[0] |= tag & 0x3f;
    std::memcpy(encodedBytes + 1, &value, sizeof(long));

    if (isBigEndianSystem()) {
        reverseSubarray(encodedBytes, 1, encodedSize);
    }

    std::string encodedString(encodedBytes, encodedSize);

    delete[] encodedBytes;
    return encodedString;
}

std::string JbufUtils::encode(int tag, bool value) {
    if (value == false) {
        return "";
    }

    char encodedBytes[2] = {0, 0};
    encodedBytes[0] = ONE_BYTE_SIZE;
    encodedBytes[0] |= tag & 0x3f;
    std::memcpy(encodedBytes + 1, &value, sizeof(bool));

    return std::string(encodedBytes, 2);
}

std::string JbufUtils::encode(int tag, unsigned int value) {
    if (value == 0) {
        return "";
    }

    int encodedSize = 5;
    char* encodedBytes = new char[encodedSize];
    encodedBytes[0] = FOUR_BYTE_SIZE;
    encodedBytes[0] |= tag & 0x3f;
    std::memcpy(encodedBytes + 1, &value, sizeof(unsigned int));

    if (isBigEndianSystem()) {
        reverseSubarray(encodedBytes, 1, encodedSize);
    }

    std::string encodedString(encodedBytes, encodedSize);

    delete[] encodedBytes;
    return encodedString;
}

std::string JbufUtils::encode(int tag, unsigned long value) {
    if (value == 0) {
        return "";
    }
    
    int encodedSize = 9;
    char* encodedBytes = new char[encodedSize];

    encodedBytes[0] = EIGHT_BYTE_SIZE;
    encodedBytes[0] |= tag & 0x3f;
    std::memcpy(encodedBytes + 1, &value, sizeof(unsigned long));

    if (isBigEndianSystem()) {
        reverseSubarray(encodedBytes, 1, encodedSize);
    }

    std::string encodedString(encodedBytes, encodedSize);

    delete[] encodedBytes;
    return encodedString;
}

JbufObject::~JbufObject() {
    for (std::unordered_map<int, JbufValue*>::iterator it = map.begin(); it != map.end(); it++) {
        delete it->second;
    }
    map.clear();
}

JbufValue* JbufObject::getValue(int tag) {
    if (map.find(tag) == map.end()) {
        map.insert({tag, new JbufValue(JBUF_DATA_SIZE_EMPTY)});
    }

    return map.at(tag);
}

void JbufObject::insertValue(int tag, JBUF_DATA_SIZE size, std::string value) {
    if (map.find(tag) == map.end()) {
        map.insert({tag, new JbufValue(size)});
    }

    if (map.at(tag)->dataSize != size) {
        throw JbufError("Different data types encoded with duplicate tag: " + tag);
    }
    else {
        map.at(tag)->entries.push_back(value);
    }
}

JbufError::JbufError(std::string message) {
    this->message = message;
}

std::string JbufError::getMessage() {
    return message;
}

JbufValue::JbufValue(JBUF_DATA_SIZE size) {
    dataSize = size;
}

int JbufValue::decodeToInt32() {
    const int DATA_SIZE = 4;
    if (dataSize != JBUF_DATA_SIZE_FOUR_BYTES || entries.size() <= 0) {
        return 0;
    }
    else {
        int value;
        char bytes[DATA_SIZE];

        memcpy(bytes, entries.at(0).c_str(), DATA_SIZE);
        
        if (JbufUtils::isBigEndianSystem()) {
            JbufUtils::reverseSubarray(bytes, 0, DATA_SIZE);
        }

        memcpy(&value, bytes, DATA_SIZE);

        return value;
    }
}

unsigned int JbufValue::decodeToUint32() {
    const int DATA_SIZE = 4;
    if (dataSize != JBUF_DATA_SIZE_FOUR_BYTES || entries.size() <= 0) {
        return 0;
    }
    else {
        unsigned int value;
        char bytes[DATA_SIZE];

        memcpy(bytes, entries.at(0).c_str(), DATA_SIZE);
        
        if (JbufUtils::isBigEndianSystem()) {
            JbufUtils::reverseSubarray(bytes, 0, DATA_SIZE);
        }

        memcpy(&value, bytes, DATA_SIZE);

        return value;
    }
}

double JbufValue::decodeToDouble() {
    const int DATA_SIZE = 8;
    if (dataSize != JBUF_DATA_SIZE_EIGHT_BYTES || entries.size() <= 0) {
        return 0.0;
    }
    else {
        double value;
        char bytes[DATA_SIZE];

        memcpy(bytes, entries.at(0).c_str(), DATA_SIZE);
        
        if (JbufUtils::isBigEndianSystem()) {
            JbufUtils::reverseSubarray(bytes, 0, DATA_SIZE);
        }

        memcpy(&value, bytes, DATA_SIZE);

        return value;
    }
}

long JbufValue::decodeToInt64() {
    const int DATA_SIZE = 8;
    if (dataSize != JBUF_DATA_SIZE_EIGHT_BYTES || entries.size() <= 0) {
        return 0;
    }
    else {
        long value;
        char bytes[DATA_SIZE];

        memcpy(bytes, entries.at(0).c_str(), DATA_SIZE);
        
        if (JbufUtils::isBigEndianSystem()) {
            JbufUtils::reverseSubarray(bytes, 0, DATA_SIZE);
        }

        memcpy(&value, bytes, DATA_SIZE);

        return value;
    }
}

unsigned long JbufValue::decodeToUint64() {
    const int DATA_SIZE = 8;
    if (dataSize != JBUF_DATA_SIZE_EIGHT_BYTES || entries.size() <= 0) {
        return 0;
    }
    else {
        unsigned long value;
        char bytes[DATA_SIZE];

        memcpy(bytes, entries.at(0).c_str(), DATA_SIZE);
        
        if (JbufUtils::isBigEndianSystem()) {
            JbufUtils::reverseSubarray(bytes, 0, DATA_SIZE);
        }

        memcpy(&value, bytes, DATA_SIZE);

        return value;
    }
}

std::string JbufValue::decodeToBytes() {
    if (dataSize != JBUF_DATA_SIZE_VARIABLE || entries.size() <= 0) {
        return "";
    }
    else {
        return entries.at(0);
    }
}

std::string JbufValue::decodeToString() {
    return decodeToBytes();
}

bool JbufValue::decodeToBool() {
    const int DATA_SIZE = 1;
    if (dataSize != JBUF_DATA_SIZE_ONE_BYTE || entries.size() <= 0) {
        return false;
    }
    else {
        bool value;
        memcpy(&value, entries.at(0).c_str(), DATA_SIZE);
        return value;
    }
}

std::vector<int> JbufValue::decodeToInt32List() {
    const int DATA_SIZE = 4;
    std::vector<int> decodedList;

    if (dataSize == JBUF_DATA_SIZE_FOUR_BYTES) {
        for (unsigned int i = 0; i < entries.size(); i++) {
            int value;
            char bytes[DATA_SIZE];

            memcpy(bytes, entries.at(i).c_str(), DATA_SIZE);
            
            if (JbufUtils::isBigEndianSystem()) {
                JbufUtils::reverseSubarray(bytes, 0, DATA_SIZE);
            }

            memcpy(&value, bytes, DATA_SIZE);
            decodedList.push_back(value);
        }
    }

    return decodedList;
}

std::vector<unsigned int> JbufValue::decodeToUint32List() {
    const int DATA_SIZE = 4;
    std::vector<unsigned int> decodedList;

    if (dataSize == JBUF_DATA_SIZE_FOUR_BYTES) {
        for (unsigned int i = 0; i < entries.size(); i++) {
            unsigned int value;
            char bytes[DATA_SIZE];

            memcpy(bytes, entries.at(i).c_str(), DATA_SIZE);
            
            if (JbufUtils::isBigEndianSystem()) {
                JbufUtils::reverseSubarray(bytes, 0, DATA_SIZE);
            }

            memcpy(&value, bytes, DATA_SIZE);
            decodedList.push_back(value);
        }
    }

    return decodedList;
}

std::vector<double> JbufValue::decodeToDoubleList() {
    const int DATA_SIZE = 8;
    std::vector<double> decodedList;

    if (dataSize == JBUF_DATA_SIZE_FOUR_BYTES) {
        for (unsigned int i = 0; i < entries.size(); i++) {
            double value;
            char bytes[DATA_SIZE];

            memcpy(bytes, entries.at(i).c_str(), DATA_SIZE);
            
            if (JbufUtils::isBigEndianSystem()) {
                JbufUtils::reverseSubarray(bytes, 0, DATA_SIZE);
            }

            memcpy(&value, bytes, DATA_SIZE);
            decodedList.push_back(value);
        }
    }

    return decodedList;
}

std::vector<long> JbufValue::decodeToInt64List() {
    const int DATA_SIZE = 8;
    std::vector<long> decodedList;

    if (dataSize == JBUF_DATA_SIZE_FOUR_BYTES) {
        for (unsigned int i = 0; i < entries.size(); i++) {
            long value;
            char bytes[DATA_SIZE];

            memcpy(bytes, entries.at(i).c_str(), DATA_SIZE);
            
            if (JbufUtils::isBigEndianSystem()) {
                JbufUtils::reverseSubarray(bytes, 0, DATA_SIZE);
            }

            memcpy(&value, bytes, DATA_SIZE);
            decodedList.push_back(value);
        }
    }

    return decodedList;
}

std::vector<unsigned long> JbufValue::decodeToUint64List() {
    const int DATA_SIZE = 8;
    std::vector<unsigned long> decodedList;

    if (dataSize == JBUF_DATA_SIZE_FOUR_BYTES) {
        for (unsigned int i = 0; i < entries.size(); i++) {
            unsigned long value;
            char bytes[DATA_SIZE];

            memcpy(bytes, entries.at(i).c_str(), DATA_SIZE);
            
            if (JbufUtils::isBigEndianSystem()) {
                JbufUtils::reverseSubarray(bytes, 0, DATA_SIZE);
            }

            memcpy(&value, bytes, DATA_SIZE);
            decodedList.push_back(value);
        }
    }

    return decodedList;
}

std::vector<std::string> JbufValue::decodeToBytesList() {
    std::vector<std::string> decodedList;

    if (dataSize == JBUF_DATA_SIZE_VARIABLE) {
        for (unsigned int i = 0; i < entries.size(); i++) {
            decodedList.push_back(entries.at(i));
        }
    }

    return decodedList;
}

std::vector<std::string> JbufValue::decodeToStringList() {
    return decodeToBytesList();
}

std::vector<bool> JbufValue::decodeToBoolList() {
    const int DATA_SIZE = 1;
    std::vector<bool> decodedList;

    if (dataSize == JBUF_DATA_SIZE_ONE_BYTE) {
        for (unsigned int i = 0; i < entries.size(); i++) {
            bool value;
            memcpy(&value, entries.at(i).c_str(), DATA_SIZE);
            decodedList.push_back(value);
        }
    }

    return decodedList;
}

JbufParser::JbufParser(std::string input) {
    this->input = input;
    this->object = new JbufObject();
    index = 0;

    while (index < input.length()) {
        // read tag
        int tag = readTag();
        // read sizeType
        JBUF_DATA_SIZE dataSizeType = readSizeType();
        // advance index
        index++;

        // if needed, read variable size
        unsigned long dataSize = readDataSize(dataSizeType);

        // copy bytes and insert into object
        if (index + dataSize > input.length()) {
            throw JbufError("Parsing error: Got field with unexpected length at index " + index);
        }

        char* data = new char[dataSize];
        std::memcpy(data, input.c_str() + index, dataSize);
        object->insertValue(tag, dataSizeType, std::string(data, dataSize));
        delete[] data;

        // advance index
        index += dataSize;
    }
}

int JbufParser::readTag() {
    char tag = input.at(index);
    tag &= 0x3f;
    return tag;
}

JBUF_DATA_SIZE JbufParser::readSizeType() {
    char tag = input.at(index);
    tag = tag >> 6;

    switch (tag) {
        case 0:
            return JBUF_DATA_SIZE_VARIABLE;
        case 1:
            return JBUF_DATA_SIZE_ONE_BYTE;
        case 2:
            return JBUF_DATA_SIZE_FOUR_BYTES;
        case 3:
            return JBUF_DATA_SIZE_EIGHT_BYTES;
        default:
            // It shouldn't be possible to reach this
            throw JbufError("Parsing error: unexpected data size encoded");
    };
}

unsigned long JbufParser::readDataSize(JBUF_DATA_SIZE sizeType) {
    switch (sizeType) {
        case JBUF_DATA_SIZE_ONE_BYTE:
            return 1;
        case JBUF_DATA_SIZE_FOUR_BYTES:
            return 4;
        case JBUF_DATA_SIZE_EIGHT_BYTES:
            return 8;
        case JBUF_DATA_SIZE_VARIABLE:
            return readVariableDataSize();
        default:
            throw JbufError("Not yet implemented");
    }
}

unsigned long JbufParser::readVariableDataSize() {
    char moreBits = 0;
    unsigned long dataSize = 0;

    do {
        if (index >= input.length()) {
            throw JbufError("Parse Error: invalid field length");
        }
        moreBits = input.at(index) & 0x80;
        dataSize = dataSize << 7;
        dataSize |= input.at(index) & 0x7f;
        index++;
    } while (moreBits);

    return dataSize;
}

JbufParser::~JbufParser() {
    delete object;
}

JbufObject* JbufParser::getJbufObject() {
    return object;
}
)";
}