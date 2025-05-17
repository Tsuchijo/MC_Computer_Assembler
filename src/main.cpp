#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
int main(int argc, char* argv[]) {

    // Check if the file exists
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "File not found: " << argv[1] << std::endl;
        return 1;
    }

    const std::unordered_map<std::string, std::string> opcodeTable = {
        {"XOR", "13"},
        {"JMP", "cat"},
        {"AND", "blocks"},
        {"LD", "chirp"},
        {"OR", "far"},
        {"OUT", "mall"},
        {"NOT", "mellohi"},
        {"DA1", "stal"},
        {"DA2", "strad"},
        {"DA3", "ward"},
        {"DA4", "11"},
        {"DA5", "wait"},
        {"DA6", "Pigstep"},
        {"DA7", "otherside"},
        {"DA8", "5"}
    };

    std::string line;
    std::vector<std::string> instructions;
    while (std::getline(file, line)) {
        // Remove comments
        size_t commentPos = line.find(';');
        if (commentPos != std::string::npos) {
            line = line.substr(0, commentPos);
        }

        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (!line.empty()) {
            instructions.push_back(line);
        }
    }
    file.close();
    std::ofstream outputFile("output.txt");
    if (!outputFile) {
        std::cerr << "Error creating output file." << std::endl;
        return 1;
    }
    for (const auto& instruction : instructions) {
        std::string opcode = instruction.substr(0, instruction.find(' '));
        auto it = opcodeTable.find(opcode);
        if (it != opcodeTable.end()) {
            outputFile << it->second << std::endl;
        } else {
            outputFile << "Unknown opcode: " << opcode << std::endl;
        }
    }
    outputFile.close();
    std::cout << "Output written to output.txt" << std::endl;
    return 0;
}