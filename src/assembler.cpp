#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "assembler.h"



Assembler::Assembler(const std::string& inputFile) {
    // Initialize the opcode table
    opcodeTable = {
        {"NOT", "13"},
        {"SKZ", "cat"},
        {"OR", "blocks"},
        {"LD", "chirp"},
        {"XOR", "far"},
        {"OUT", "mall"},
        {"AND", "mellohi"},
        {"DA1", "stal"},
        {"DA2", "strd"},
        {"DA3", "ward"},
        {"DA4", "11"},
        {"DA5", "wait"},
        {"DA6", "pigstep"},
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
        {"NOT", "13"},
        {"SKZ", "cat"},
        {"OR", "blocks"},
        {"LD", "chirp"},
        {"XOR", "far"},
        {"OUT", "mall"},
        {"AND", "mellohi"},
        {"DA1", "stal"},
        {"DA2", "strd"},
        {"DA3", "ward"},
        {"DA4", "11"},
        {"DA5", "wait"},
        {"DA6", "pigstep"},
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
    auto end = assemblyFile.end();
    auto it = assemblyFile.begin();
    std::vector<std::string> newAssemblyFile;
    while (it != end) {
        std::string line = *it;
        removeComments(line);
        trimWhitespace(line);
        if (!line.empty()) {
            if (line.substr(0, 4) == "def ") {
                parseMacroDefinition(it, end);
                // Don't increment it here - parseMacroDefinition already moved it
            } else {
                newAssemblyFile.push_back(line);
                ++it;  // Only increment for non-macro lines
            }
        } else {
            ++it;  // Increment for empty lines
        }
    }
    assemblyFile = newAssemblyFile;  // Update the assembly file with the new lines

    std::cout << "Second pass: parsing macro invocations and generating instructions." << std::endl;
    // Second pass: parse macro invocations
    bool containsMacroInvocation = true;
    int nestedMacroDepth = 0;
    // Loop until no more macro invocations are found, to handle nested macros
    while(containsMacroInvocation && nestedMacroDepth < MAX_NESTED_MACRO_DEPTH) {
        std::vector<std::string> instructions;
        containsMacroInvocation = false;
        
        for (size_t i = 0; i < assemblyFile.size(); ++i) {
            const std::string& line = assemblyFile[i];
            
            // Check for SKZ followed by macro invocation pattern
            if (line == "SKZ" && i + 1 < assemblyFile.size() && isValidMacroInvocation(assemblyFile[i + 1])) {
                // Skip the SKZ line and process the macro with SKZ insertion
                ++i; // Move to the macro line
                parseMacroInvocation(assemblyFile[i], instructions, true);
                containsMacroInvocation = true;
            }
            // Check if the line is a macro invocation
            else if (isValidMacroInvocation(line)) {
                parseMacroInvocation(line, instructions, false);
                containsMacroInvocation = true;
            } else {
                // Check if the line is a valid opcode
                if (isValidOpcode(line)) {
                    instructions.push_back(line);
                } else {
                    std::cerr << "Error: Invalid opcode or macro invocation: " << line << std::endl;
                }
            }
        }
        assemblyFile = instructions;  // Use the generated instructions for the next pass
        ++nestedMacroDepth;
    }
    if (nestedMacroDepth == MAX_NESTED_MACRO_DEPTH) {
        std::cerr << "Error: Maximum nested macro depth exceeded." << std::endl;
    }
    // Generate the final instructions
    for (const auto& line : assemblyFile) {
        // Check if the line is a valid opcode
        if (isValidOpcode(line)) {
            discInstructions.push_back(line);
        } else {
            std::cerr << "Error: Invalid opcode or macro invocation: " << line << std::endl;
        }
    }
    
    // Enforce ISA constraint: program must be a multiple of 27 instructions
    const int INSTRUCTION_MULTIPLE = 27;
    
    int currentSize = discInstructions.size();
    int remainder = currentSize % INSTRUCTION_MULTIPLE;
    
    if (remainder != 0) {
        int nopsNeeded = INSTRUCTION_MULTIPLE - remainder;
        for (int i = 0; i < nopsNeeded; ++i) {
            discInstructions.push_back("NOT");
        }
        std::cout << "Program padded from " << currentSize << " to " << (currentSize + nopsNeeded) << " instructions (" << nopsNeeded << " NOPs added)" << std::endl;
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

bool Assembler::isValidOpcode(const std::string& line) {
    // checks across opcode table and macro table
    auto it = opcodeTable.find(line);
    if (it != opcodeTable.end()) {
        return true;  // Found in opcode table
    }
    // Check if it's a macro name
    if (macroTable.find(line) != macroTable.end()) {
        return true;  // Found in macro table
    }
    return false;  // Not found in either table
}

bool Assembler::isValidMacroParameter(const std::string& line, const MacroDefinition& macro) {
    // Check if the line is a valid macro parameter
    for (const auto& param : macro.parameters) {
        if (line == param) {
            return true;  // Found in macro parameters
        }
    }
    return false;
}

bool Assembler::isValidMacroInvocation(const std::string& line) {
    // Check if the line is a valid macro invocation
    size_t openParenPos = line.find('(');
    size_t closeParenPos = line.find(')');
    if (openParenPos != std::string::npos && closeParenPos != std::string::npos && openParenPos < closeParenPos) {
        std::string macroName = line.substr(0, openParenPos);
        return macroTable.find(macroName) != macroTable.end();  // Found in macro table
    }
    return false;  // Not a valid macro invocation
}

bool Assembler::isMacroInvocation(const std::string& line) {
    // Check if the line is fomatted as a macro invocation, e.g., "macroName(param1, param2)"
    // Does not check if the macro exists
    size_t openParenPos = line.find('(');
    size_t closeParenPos = line.find(')');
    // check if it has a valid format
    if (openParenPos != std::string::npos && closeParenPos != std::string::npos && openParenPos < closeParenPos) {
        return true;  // Valid macro invocation format
    }
    return false;  // Not a valid macro invocation format
}

void Assembler::writeOutput(const std::string& outputFile) {
    std::ofstream file(outputFile);
    if (!file) {
        std::cerr << "Error creating output file." << std::endl;
        return;
    }
    for (const auto& instruction : discInstructions) {
        auto it = opcodeTable.find(instruction);
        file << it->second << std::endl;
    }
    file.close();
}

void Assembler::writeOutputCommand(const std::string& outputFile) {
    std::ofstream file(outputFile);
    if (!file) {
        std::cerr << "Error creating output file." << std::endl;
        return;
    }
    
    const int MAX_ITEMS_PER_SHULKER = 27;
    int totalInstructions = discInstructions.size();
    int shulkerCount = (totalInstructions + MAX_ITEMS_PER_SHULKER - 1) / MAX_ITEMS_PER_SHULKER;
    
    for (int shulkerIndex = 0; shulkerIndex < shulkerCount; ++shulkerIndex) {
        int startIdx = shulkerIndex * MAX_ITEMS_PER_SHULKER;
        int endIdx = std::min(startIdx + MAX_ITEMS_PER_SHULKER, totalInstructions);
        
        std::string shulkerName = "Program";
        if (shulkerCount > 1) {
            shulkerName += "_Part_" + std::to_string(shulkerIndex + 1);
        }
        
        file << "/give @p shulker_box{display:{Name:'{\"text\":\"" << shulkerName << "\"}'},BlockEntityTag:{Items:[";
        
        for (int i = startIdx; i < endIdx; ++i) {
            auto it = opcodeTable.find(discInstructions[i]);
            if (it != opcodeTable.end()) {
                if (i > startIdx) file << ",";
                file << "{Slot:" << (i - startIdx) << "b,id:\"minecraft:music_disc_" << it->second << "\",Count:1b}";
            }
        }
        
        file << "]}}" << std::endl;
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
        std::cerr << "Error: Invalid macro definition format. Expected: def macroName(param1, param2, ...)" << std::endl;
        ++currentLine;
        return;
    }
    
    // Remove the "def " prefix
    std::string defLine = line.substr(4);
    
    // Find the opening parenthesis
    size_t openParenPos = defLine.find('(');
    if (openParenPos == std::string::npos) {
        std::cerr << "Error: Invalid macro definition format. Expected: def macroName(param1, param2, ...)" << std::endl;
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
            std::cout << "End of macro definition: " << macro.name << std::endl;
            break;  // End of macro definition
        }
        // Check and make sure line is either a parameter or a valid instruction
        if (!isValidMacroParameter(*currentLine, macro) && !isValidOpcode(*currentLine) && !isMacroInvocation(*currentLine)) {
            std::cerr << "Error: Invalid instruction in macro body: " << *currentLine << std::endl;
            return;
        }
        macro.body.push_back(*currentLine);
        ++currentLine;
    }
    
    // Make sure we found the "end" marker
    if (currentLine == end) {
        std::cerr << "Error: Missing 'end' marker for macro definition" << std::endl;
        return;
    }
    
    // Add to macro table
    macroTable[macro.name] = macro;
    
    // Skip the "end" line
    ++currentLine;
}

void Assembler::findParameters(const std::string& line, std::vector<std::string>& parameters) {
    // Implementation for finding parameters in a macro definition
    size_t openParenPos = line.find('(');
    size_t closeParenPos = line.find(')');
    if (openParenPos != std::string::npos && closeParenPos != std::string::npos && openParenPos < closeParenPos) {
        std::string paramList = line.substr(openParenPos + 1, closeParenPos - openParenPos - 1);
        std::istringstream paramStream(paramList);
        std::string param;
        while (std::getline(paramStream, param, ',')) {
            trimWhitespace(param);
            if (!param.empty()) {
                parameters.push_back(param);
            }
        }
    }
}

void Assembler::parseMacroInvocation(const std::string& line, std::vector<std::string>& instructions, bool insertSKZ) {
    // Implementation for parsing macro invocations
    std::string macroName = line.substr(0, line.find('('));
    auto it = macroTable.find(macroName);
    if (it != macroTable.end()) {
        MacroDefinition& macro = it->second;
        std::vector<std::string> arguments;
        findParameters(line, arguments);

        // trim whitespace from arguments and make sure they match the macro parameters
        for (auto& arg : arguments) {
            trimWhitespace(arg);
            if (arguments.size() > macro.parameters.size()) {
                std::cerr << "Error: Too many arguments for macro " << macroName << std::endl;
                return;
            }
            else if (arguments.size() < macro.parameters.size()) {
                std::cerr << "Error: Not enough arguments for macro " << macroName << std::endl;
                return;
            }
            if (!isValidOpcode(arg) && !isValidMacroInvocation(arg)) {
                std::cerr << "Error: Invalid argument for macro " << macroName << ": " << arg << std::endl;
                return;
            }
        }

        // Replace parameters in the macro body with arguments
        std::vector<std::string> expandedBody = macro.body; // Work with a copy
        for(auto& bodyLine : expandedBody) {
            // For each line, if is not a macro call just check and replace if it is a parameter alone
            if (!isValidMacroInvocation(bodyLine) && !isValidOpcode(bodyLine)) {
                for (size_t i = 0; i < macro.parameters.size(); ++i) {
                    std::string param = macro.parameters[i];
                    std::string arg = arguments[i];
                    size_t pos = bodyLine.find(param);
                    if (pos != std::string::npos) {
                        bodyLine.replace(pos, param.length(), arg);
                    }
                }
            }
            else if (isValidMacroInvocation(bodyLine)) {
                std::vector<std::string> nestedParamNames;
                findParameters(bodyLine, nestedParamNames);
                for (size_t i = 0; i < nestedParamNames.size(); ++i) {
                    for (size_t j = 0; j < macro.parameters.size(); ++j) {
                        std::string param = macro.parameters[j];
                        std::string nestedParam = nestedParamNames[i];
                        std::string arg = arguments[j];
                        // only search after the first '('
                        size_t searchStart = bodyLine.find(nestedParam, bodyLine.find('('));
                        size_t pos = bodyLine.find(nestedParam, searchStart);
                        if (pos != std::string::npos) {
                            bodyLine.replace(pos, nestedParam.length(), arg);
                        }
                    }
                }
                
            }
        }

        // Add the expanded macro body to the instructions
        for (size_t i = 0; i < expandedBody.size(); ++i) {
            const auto& bodyLine = expandedBody[i];
            std::cout << "Adding line to instructions: " << bodyLine << std::endl;
            instructions.push_back(bodyLine);
            
            // Insert SKZ between lines if requested (but not after the last line)
            if (insertSKZ && i < expandedBody.size() - 1) {
                std::cout << "Adding SKZ between macro lines" << std::endl;
                instructions.push_back("SKZ");
            }
        }
    } else {
        std::cerr << "Unknown macro: " << macroName << std::endl;
    }
    
}