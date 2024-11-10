#include "./Processors/Program.h"
#include "./JsonUtils.h"

#include <iostream>
#include <string>


int main(int argc, char** argv) {
    Program program;
    return program.run(argc, argv);
}