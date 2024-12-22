#include "Utils/JbufUtils.h"

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
    char encodedBytes[2] = {0, 0};
    encodedBytes[0] = ONE_BYTE_SIZE;
    encodedBytes[0] |= tag & 0x3f;
    std::memcpy(encodedBytes + 1, &value, sizeof(bool));

    return std::string(encodedBytes, 2);
}

std::string JbufUtils::encode(int tag, unsigned int value) {
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
