#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "assembler.h"


Assembler::Assembler(const std::string& inputFile) {
    // Initialize the opcode table
    opcodeTable = {
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

    // Read the assembly file
    if (Assembler::readAssemblyFile(inputFile)) {
        std::cout << "File read successfully." << std::endl;
    } else {
        std::cerr << "Error reading file." << std::endl;
        return;
    }
    // Assemble the instructions
    Assembler::assemble();
}

Assembler::Assembler() {
    // Initialize the opcode table
    opcodeTable = {
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
}

bool Assembler::readAssemblyFile(const std::string& inputFile) {
    std::ifstream file(inputFile);
    if (!file) {
        std::cerr << "File not found: " << inputFile << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        removeComments(line);
        trimWhitespace(line);
        if (!line.empty()) {
            assemblyFile.push_back(line);
        }
    }
    file.close();
    return true;
}

void Assembler::assemble(){

    // First pass: parse macro definitions, remove comments, and trim whitespace
    auto it = assemblyFile.begin();
    auto end = assemblyFile.end();
    while (it != end) {
        parseMacroDefinition(it, end);
        removeComments(*it);
        trimWhitespace(*it);
        ++it;
    }

    // Second pass: parse macro invocations and generate instructions
    for (const auto& line : assemblyFile) {
        // Check if the line is a macro invocation
        if (line.find('(') != std::string::npos && line.find(')') != std::string::npos) {
            parseMacroInvocation(line);
        } else {
            // Check if the line is an opcode
            auto it = opcodeTable.find(line);
            if (it != opcodeTable.end()) {
                instructions.push_back(it->second);
            } else {
                std::cerr << "Unknown instruction: " << line << std::endl;
            }
        }
    }
}

void Assembler::removeComments(std::string& line) {
    size_t commentPos = line.find(';');
    if (commentPos != std::string::npos) {
        line = line.substr(0, commentPos);
    }
}

void Assembler::trimWhitespace(std::string& line) {
    line.erase(0, line.find_first_not_of(" \t"));
    line.erase(line.find_last_not_of(" \t") + 1);
}

void Assembler::writeOutput(const std::string& outputFile) {
    std::ofstream file(outputFile);
    if (!file) {
        std::cerr << "Error creating output file." << std::endl;
        return;
    }
    for (const auto& instruction : instructions) {
        file << instruction << std::endl;
    }
    file.close();
}

void Assembler::writeOutputCommand(const std::string& outputFile) {
    std::ofstream file(outputFile);
    if (!file) {
        std::cerr << "Error creating output file." << std::endl;
        return;
    }
    for (const auto& instruction : instructions) {
        file << instruction << std::endl;
    }
    file.close();
}

void Assembler::parseMacroDefinition(std::vector<std::string>::iterator& currentLine, 
                                    const std::vector<std::string>::iterator& end) {
    // Extract the macro name and parameters from the def directive line
    // Format: def macroName(param1, param2, param3)
    std::string line = *currentLine;
    
    // Check if line starts with "def "
    if (line.substr(0, 4) != "def ") {
        // Not a macro definition, skip it
        ++currentLine;
        return;
    }
    
    // Remove the "def " prefix
    std::string defLine = line.substr(4);
    
    // Find the opening parenthesis
    size_t openParenPos = defLine.find('(');
    if (openParenPos == std::string::npos) {
        std::cerr << "Error: Invalid macro definition format. Expected: def macroName(param1, param2, ...)" << std::endl;
        ++currentLine;
        return;
    }
    
    // Extract the macro name
    MacroDefinition macro;
    macro.name = defLine.substr(0, openParenPos);
    trimWhitespace(macro.name);
    
    // Find the closing parenthesis
    size_t closeParenPos = defLine.find(')', openParenPos);
    if (closeParenPos == std::string::npos) {
        std::cerr << "Error: Missing closing parenthesis in macro definition" << std::endl;
        ++currentLine;
        return;
    }
    
    // Extract the parameter list
    std::string paramList = defLine.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
    
    // Parse the comma-separated parameter list
    std::istringstream paramStream(paramList);
    std::string param;
    while (std::getline(paramStream, param, ',')) {
        trimWhitespace(param);
        if (!param.empty()) {
            macro.parameters.push_back(param);
        }
    }
    
    // Collect the macro body until "end"
    ++currentLine;  // Move past the def line
    while (currentLine != end) {
        if (*currentLine == "end") {
            break;  // End of macro definition
        }
        // Check and make sure line is either a parameter or a valid instruction
        if (opcodeTable.find(*currentLine) == opcodeTable.end()) {
            bool isParameter = false;
            for (const auto& param : macro.parameters) {
                if (*currentLine == param) {
                    isParameter = true;
                    break;
                }
            }
            if (!isParameter) {
                std::cerr << "Error: Invalid line in macro body: " << *currentLine << std::endl;
                ++currentLine;
                continue;  // Skip invalid lines
            }
        }
        macro.body.push_back(*currentLine);
        ++currentLine;
    }
    
    // Make sure we found the "end" marker
    if (currentLine == end) {
        return;
    }
    
    // Add to macro table
    macroTable[macro.name] = macro;
    
    // Skip the "end" line
    ++currentLine;
}

void Assembler::parseMacroInvocation(const std::string& line) {
    // Implementation for parsing macro invocations
    std::string macroName = line.substr(0, line.find('('));
    auto it = macroTable.find(macroName);
    if (it != macroTable.end()) {
        MacroDefinition& macro = it->second;
        std::vector<std::string> arguments;
        std::string params = line.substr(line.find('(') + 1, line.find(')') - line.find('(') - 1);
        size_t pos = 0;
        while ((pos = params.find(',')) != std::string::npos) {
            arguments.push_back(params.substr(0, pos));
            params.erase(0, pos + 1);
        }
        arguments.push_back(params); // Add the last argument

        // Replace parameters in the macro body with arguments
        for (size_t i = 0; i < macro.parameters.size(); ++i) {
            for (auto& bodyLine : macro.body) {
                size_t paramPos = bodyLine.find(macro.parameters[i]);
                if (paramPos != std::string::npos) {
                    bodyLine.replace(paramPos, macro.parameters[i].length(), arguments[i]);
                }
            }
        }

        // Add the expanded macro body to the instructions
        for (const auto& bodyLine : macro.body) {
            instructions.push_back(bodyLine);
        }
    } else {
        std::cerr << "Unknown macro: " << macroName << std::endl;
    }
    
}