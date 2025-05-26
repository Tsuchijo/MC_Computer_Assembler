#include "emulator.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>

Emulator::Emulator() {
    initializeOpcodeMap();
    reset();
}

void Emulator::initializeOpcodeMap() {
    opcodeToNumber = {
        {"XOR", 1}, {"JMP", 2}, {"AND", 3}, {"LD", 4},
        {"OR", 5}, {"OUT", 6}, {"NOT", 7}, {"DA1", 8},
        {"DA2", 9}, {"DA3", 10}, {"DA4", 11}, {"DA5", 12},
        {"DA6", 13}, {"DA7", 14}, {"DA8", 15}
    };
}

bool Emulator::loadProgram(const std::string& assemblyFile) {
    Assembler assembler;
    if (!assembler.readAssemblyFile(assemblyFile)) {
        std::cerr << "Failed to read assembly file: " << assemblyFile << std::endl;
        return false;
    }
    
    assembler.assemble();
    instructions = assembler.getInstructions();
    
    if (instructions.empty()) {
        std::cerr << "No instructions found in assembly file." << std::endl;
        return false;
    }
    
    reset();
    std::cout << "Loaded program with " << instructions.size() << " instructions." << std::endl;
    return true;
}

void Emulator::reset() {
    registerValue = false;
    selectedDataLine = 0;
    programCounter = 0;
    outputFlag = false;
    halted = false;
    
    for (int i = 0; i < 8; ++i) {
        dataLines[i].input = false;
        dataLines[i].output = false;
    }
}

bool Emulator::step() {
    if (halted || programCounter >= static_cast<int>(instructions.size())) {
        halted = true;
        return false;
    }
    
    std::string instruction = instructions[programCounter];
    std::cout << "PC:" << std::setw(3) << programCounter 
              << " | " << std::setw(8) << instruction 
              << " | ";
    
    bool continueExecution = executeInstruction(instruction);
    
    if (!continueExecution) {
        halted = true;
        return false;
    }
    
    if (outputFlag) {
        updateOutput();
        outputFlag = false;
    }
    
    programCounter++;
    
    std::cout << "REG:" << (registerValue ? 1 : 0)
              << " | DA" << (selectedDataLine + 1) 
              << " IN:" << (dataLines[selectedDataLine].input ? 1 : 0)
              << " OUT:" << (dataLines[selectedDataLine].output ? 1 : 0)
              << std::endl;
    
    return true;
}

void Emulator::run() {
    std::cout << "Running program..." << std::endl;
    printState();
    std::cout << std::endl;
    
    while (step()) {
        // Continue execution
    }
    
    if (halted) {
        std::cout << std::endl << "Program halted." << std::endl;
    } else {
        std::cout << std::endl << "Program completed." << std::endl;
    }
    
    printState();
}

void Emulator::runInteractive() {
    clearScreen();
    std::cout << "Interactive mode commands:" << std::endl;
    std::cout << "  Enter/step - Execute next instruction" << std::endl;
    std::cout << "  q/quit     - Quit emulator" << std::endl;
    std::cout << "  r/run      - Run until completion" << std::endl;
    std::cout << "  s/state    - Show current state" << std::endl;
    std::cout << "  p/program  - Show program with PC" << std::endl;
    std::cout << "  set DAx 0/1- Set data line x input to 0 or 1" << std::endl;
    std::cout << "  h/help     - Show this help" << std::endl;
    std::cout << std::endl;
    printState();
    std::cout << std::endl;
    
    std::string input;
    while (isRunning()) {
        std::cout << ">>> ";
        std::getline(std::cin, input);
        
        if (input == "q" || input == "quit") {
            break;
        } else if (input == "r" || input == "run") {
            clearScreen();
            std::cout << "Running program..." << std::endl;
            printState();
            std::cout << std::endl;
            while (step()) {
                // Continue execution
            }
            break;
        } else if (input == "s" || input == "state") {
            clearScreen();
            printState();
        } else if (input == "p" || input == "program") {
            clearScreen();
            printProgram();
        } else if (input == "h" || input == "help") {
            clearScreen();
            std::cout << "Interactive mode commands:" << std::endl;
            std::cout << "  Enter/step - Execute next instruction" << std::endl;
            std::cout << "  q/quit     - Quit emulator" << std::endl;
            std::cout << "  r/run      - Run until completion" << std::endl;
            std::cout << "  s/state    - Show current state" << std::endl;
            std::cout << "  p/program  - Show program with PC" << std::endl;
            std::cout << "  set DAx 0/1- Set data line x input to 0 or 1" << std::endl;
            std::cout << "  h/help     - Show this help" << std::endl;
            std::cout << std::endl;
            printState();
        } else if (input.substr(0, 3) == "set") {
            // Parse "set DAx value" command
            std::istringstream iss(input);
            std::string cmd, dataStr;
            int value;
            if (iss >> cmd >> dataStr >> value) {
                if (dataStr.length() == 3 && dataStr.substr(0, 2) == "DA") {
                    int dataLine = std::stoi(dataStr.substr(2)) - 1;
                    if (dataLine >= 0 && dataLine < 8) {
                        bool boolValue = (value != 0);
                        setDataInput(dataLine, boolValue);
                        clearScreen();
                        std::cout << "Set DA" << (dataLine + 1) << " input to " << (boolValue ? 1 : 0) << std::endl;
                        printState();
                    } else {
                        std::cout << "Invalid data line. Use DA1-DA8." << std::endl;
                    }
                } else {
                    std::cout << "Invalid format. Use: set DAx 0/1" << std::endl;
                }
            } else {
                std::cout << "Invalid format. Use: set DAx 0/1" << std::endl;
            }
        } else {
            clearScreen();
            step();
            printState();
        }
    }
    
    if (halted) {
        std::cout << "Program halted." << std::endl;
    } else if (!isRunning()) {
        std::cout << "Program completed." << std::endl;
    }
}

void Emulator::printState() const {
    std::cout << "=== Computer State ===" << std::endl;
    std::cout << "Program Counter: " << programCounter << std::endl;
    std::cout << "Register: " << (registerValue ? 1 : 0) << std::endl;
    std::cout << "Selected Data Line: DA" << (selectedDataLine + 1) << std::endl;
    std::cout << "Output Flag: " << (outputFlag ? "true" : "false") << std::endl;
    std::cout << "Halted: " << (halted ? "true" : "false") << std::endl;
    printDataLines();
}

void Emulator::printDataLines() const {
    std::cout << "Data Lines:" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << "  DA" << (i + 1) << ": IN=" << (dataLines[i].input ? 1 : 0)
                  << " OUT=" << (dataLines[i].output ? 1 : 0);
        if (i == selectedDataLine) {
            std::cout << " [SELECTED]";
        }
        std::cout << std::endl;
    }
}

void Emulator::printProgram() const {
    std::cout << "=== Program ===" << std::endl;
    for (size_t i = 0; i < instructions.size(); ++i) {
        std::cout << std::setw(3) << i << ": " << instructions[i];
        if (static_cast<int>(i) == programCounter) {
            std::cout << " <-- PC";
        }
        std::cout << std::endl;
    }
}

void Emulator::clearScreen() const {
    // Clear screen using ANSI escape codes
    std::cout << "\033[2J\033[H";
}

void Emulator::setDataInput(int dataLine, bool value) {
    if (dataLine >= 0 && dataLine < 8) {
        dataLines[dataLine].input = value;
    }
}

bool Emulator::getDataOutput(int dataLine) const {
    if (dataLine >= 0 && dataLine < 8) {
        return dataLines[dataLine].output;
    }
    return false;
}

bool Emulator::executeInstruction(const std::string& instruction) {
    auto it = opcodeToNumber.find(instruction);
    if (it == opcodeToNumber.end()) {
        std::cerr << "Unknown instruction: " << instruction << std::endl;
        return false;
    }
    
    int opcode = it->second;
    
    switch (opcode) {
        case 1: executeXOR(); break;
        case 2: executeJMP(); break;
        case 3: executeAND(); break;
        case 4: executeLD(); break;
        case 5: executeOR(); break;
        case 6: executeOUT(); break;
        case 7: executeNOT(); break;
        case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
            executeDataSelect(opcode - 8); break;
        default:
            std::cerr << "Invalid opcode: " << opcode << std::endl;
            return false;
    }
    
    return true;
}

void Emulator::updateOutput() {
    dataLines[selectedDataLine].output = registerValue;
}

void Emulator::executeXOR() {
    registerValue = registerValue ^ dataLines[selectedDataLine].input;
}

void Emulator::executeJMP() {
    if (programCounter < 2) {
        std::cerr << "Warning: JUMP instruction at position " << programCounter 
                  << " is less than 2 instructions from start. This may not work on the actual hardware." << std::endl;
    }
    
    if (registerValue == false) {
        programCounter = -1;  // Will be incremented to 0 at end of step()
    }
}

void Emulator::executeAND() {
    registerValue = registerValue & dataLines[selectedDataLine].input;
}

void Emulator::executeLD() {
    registerValue = dataLines[selectedDataLine].input;
}

void Emulator::executeOR() {
    registerValue = registerValue | dataLines[selectedDataLine].input;
}

void Emulator::executeOUT() {
    outputFlag = true;
}

void Emulator::executeNOT() {
    registerValue = ~registerValue;
}

void Emulator::executeDataSelect(int dataLineIndex) {
    selectedDataLine = dataLineIndex;
}

std::string Emulator::getInstructionName(const std::string& instruction) const {
    return instruction;
}