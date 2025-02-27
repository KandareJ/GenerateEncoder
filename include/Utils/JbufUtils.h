#pragma once

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
};