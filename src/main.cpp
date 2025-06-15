#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "assembler.h"
#include "emulator.h"

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] <input_file.asm>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -e, --emulate         Run in emulator mode" << std::endl;
    std::cout << "  -i, --interactive     Run in interactive emulator mode" << std::endl;
    std::cout << "  -t, --turing          Enable Turing Complete mode (with tape memory)" << std::endl;
    std::cout << "  -o, --output <file>   Specify output file (default: output.txt)" << std::endl;
    std::cout << "  -m, --minecraft       Output as minecraft commands (default: numeric)" << std::endl;
    std::cout << "  -h, --help            Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Default behavior: Assemble to numeric format in output.txt" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string inputFile;
    std::string outputFile = "output.txt";
    bool emulatorMode = false;
    bool interactiveMode = false;
    bool minecraftFormat = false;
    bool turingMode = false;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-e" || arg == "--emulate") {
            emulatorMode = true;
        } else if (arg == "-i" || arg == "--interactive") {
            emulatorMode = true;
            interactiveMode = true;
        } else if (arg == "-t" || arg == "--turing") {
            turingMode = true;
        } else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                outputFile = argv[++i];
            } else {
                std::cerr << "Error: -o/--output requires a filename" << std::endl;
                printUsage(argv[0]);
                return 1;
            }
        } else if (arg == "-m" || arg == "--minecraft") {
            minecraftFormat = true;
        } else if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg[0] != '-') {
            inputFile = arg;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }

    if (inputFile.empty()) {
        std::cerr << "No input file specified." << std::endl;
        printUsage(argv[0]);
        return 1;
    }

    // Check if the file exists
    std::ifstream file(inputFile);
    if (!file) {
        std::cerr << "File not found: " << inputFile << std::endl;
        return 1;
    }
    file.close();

    if (emulatorMode) {
        // Run emulator
        Emulator emulator;
        if (!emulator.loadProgram(inputFile)) {
            std::cerr << "Failed to load program for emulation." << std::endl;
            return 1;
        }

        // Enable Turing Complete mode if requested
        if (turingMode) {
            emulator.enableTapeMode(true);
            std::cout << "Turing Complete mode enabled - data lines function as memory and tape operations." << std::endl;
        }

        if (interactiveMode) {
            emulator.runInteractive();
        } else {
            emulator.run();
        }
    } else {
        // Run assembler (default behavior)
        Assembler assembler = Assembler(inputFile);
        
        if (minecraftFormat) {
            assembler.writeOutputCommand(outputFile);
            std::cout << "Assembly complete. Minecraft commands written to " << outputFile << std::endl;
        } else {
            assembler.writeOutput(outputFile);
            std::cout << "Assembly complete. Numeric output written to " << outputFile << std::endl;
        }
    }

    return 0;
}