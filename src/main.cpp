#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "assembler.h"
int main(int argc, char* argv[]) {

    // Check if the file exists
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "File not found: " << argv[1] << std::endl;
        return 1;
    }

    Assembler assembler = Assembler(argv[1]);

    assembler.writeOutput("output.txt");
    assembler.writeOutputCommand("outputCommand.txt");
}