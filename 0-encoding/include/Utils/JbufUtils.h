#pragma once

#include <string>
#include <cstring>

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