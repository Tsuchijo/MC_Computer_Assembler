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
        {"NOT", 1}, {"SKZ", 2}, {"OR", 3}, {"LD", 4},
        {"XOR", 5}, {"OUT", 6}, {"AND", 7}, {"DA1", 8},
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
    skipNext = false;
    tapeMode = false;
    
    for (int i = 0; i < 8; ++i) {
        dataLines[i].input = false;
        dataLines[i].output = false;
        dataLines[i].memoryValue = false;
    }
    
    tape1.tape.clear();
    tape1.headPosition = 0;
    tape2.tape.clear();
    tape2.headPosition = 0;
}

bool Emulator::step() {
    if (halted || programCounter >= static_cast<int>(instructions.size())) {
        // Check HALT flag (DA1) before declaring halt
        if (programCounter >= static_cast<int>(instructions.size()) && dataLines[0].memoryValue) {
            halted = true;
            return false;
        }
        // Loop back to start if no halt flag
        if (programCounter >= static_cast<int>(instructions.size())) {
            programCounter = 0;
        }
    }
    
    std::string instruction = instructions[programCounter];
    std::cout << "PC:" << std::setw(3) << programCounter 
              << " | " << std::setw(8) << instruction 
              << " | ";
    
    // Check if we should skip this instruction
    if (skipNext) {
        skipNext = false;
        programCounter++;
        std::cout << "SKIPPED | REG:" << (registerValue ? 1 : 0) << std::endl;
        return true;
    }
    
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
                printTapeState();
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
    std::cout << "Tape Mode: " << (tapeMode ? "enabled" : "disabled") << std::endl;
    printDataLines();
    if (tapeMode) {
        printTapeState();
    }
}

void Emulator::printDataLines() const {
    std::cout << "Data Lines:" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << "  DA" << (i + 1);
        // Show memory value for DA1 and DA2
        if (i < 2) {
            std::cout << " MEM=" << (dataLines[i].memoryValue ? 1 : 0);
        }
        else {
            std::cout << " IN=" << (dataLines[i].input ? 1 : 0)
                      << " OUT=" << (dataLines[i].output ? 1 : 0);
        }
        if (i == selectedDataLine) {
            std::cout << " [SELECTED]";
        }
        std::cout << std::endl;
    }
}

void Emulator::printTapeState() const {
    std::cout << "Tape State:" << std::endl;
    
    // Print Tape 1 state
    std::cout << "  Tape 1 (DA3-DA5): ";
    for (int i = tape1.headPosition - 3; i <= tape1.headPosition + 3; ++i) {
        if (i == tape1.headPosition) {
            std::cout << "[" << (tape1.tape.count(i) ? (tape1.tape.at(i) ? 1 : 0) : 0) << "]";
        } else {
            std::cout << (tape1.tape.count(i) ? (tape1.tape.at(i) ? 1 : 0) : 0);
        }
        if (i < tape1.headPosition + 3) std::cout << " ";
    }
    std::cout << " (Head at " << tape1.headPosition << ")" << std::endl;
    
    // Print Tape 2 state  
    std::cout << "  Tape 2 (DA6-DA8): ";
    for (int i = tape2.headPosition - 3; i <= tape2.headPosition + 3; ++i) {
        if (i == tape2.headPosition) {
            std::cout << "[" << (tape2.tape.count(i) ? (tape2.tape.at(i) ? 1 : 0) : 0) << "]";
        } else {
            std::cout << (tape2.tape.count(i) ? (tape2.tape.at(i) ? 1 : 0) : 0);
        }
        if (i < tape2.headPosition + 3) std::cout << " ";
    }
    std::cout << " (Head at " << tape2.headPosition << ")" << std::endl;
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
    if (dataLine >= 2 && dataLine < 8) {
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
        case 1: executeNOT(); break;
        case 2: executeSKZ(); break;
        case 3: executeOR(); break;
        case 4: executeLD(); break;
        case 5: executeXOR(); break;
        case 6: executeOUT(); break;
        case 7: executeAND(); break;
        case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:
            executeDataSelect(opcode - 8); break;
        default:
            std::cerr << "Invalid opcode: " << opcode << std::endl;
            return false;
    }
    
    return true;
}

void Emulator::updateOutput() {
    if (tapeMode && (selectedDataLine >= 2 && selectedDataLine <= 7)) {
        handleTapeOperation(selectedDataLine, true);
    } else {
        // Normal memory operation for DA1/DA2, output for others
        if (selectedDataLine < 2) {
            writeMemoryCell(selectedDataLine, registerValue);
        } else {
            dataLines[selectedDataLine].output = registerValue;
        }
    }
}

void Emulator::executeXOR() {
    bool valueToXOR;
    if (tapeMode && (selectedDataLine >= 2 && selectedDataLine <= 7)) {
        // For tape operations, we need to read the current tape value
        if (selectedDataLine >= 2 && selectedDataLine <= 4) {
            // Tape 1 - DA4 is the read/write head
            if (selectedDataLine == 3) {
                valueToXOR = tape1.read();
            } else {
                valueToXOR = dataLines[selectedDataLine].input;
            }
        } else if (selectedDataLine >= 5 && selectedDataLine <= 7) {
            // Tape 2 - DA7 is the read/write head  
            if (selectedDataLine == 6) {
                valueToXOR = tape2.read();
            } else {
                valueToXOR = dataLines[selectedDataLine].input;
            }
        } else {
            valueToXOR = dataLines[selectedDataLine].input;
        }
    } else {
        // Normal operation - use input line or memory
        if (selectedDataLine < 2) {
            valueToXOR = readMemoryCell(selectedDataLine);
        } else {
            valueToXOR = dataLines[selectedDataLine].input;
        }
    }
    
    registerValue = registerValue ^ valueToXOR;
}

void Emulator::executeSKZ() {
    // Skip next instruction if SKIP flag (DA2 memory) is high
    if (dataLines[1].memoryValue) {
        skipNext = true;
    }
}

void Emulator::executeAND() {
    bool valueToAND;
    if (tapeMode && (selectedDataLine >= 2 && selectedDataLine <= 7)) {
        // For tape operations, we need to read the current tape value
        if (selectedDataLine >= 2 && selectedDataLine <= 4) {
            // Tape 1 - DA4 is the read/write head
            if (selectedDataLine == 3) {
                valueToAND = tape1.read();
            } else {
                valueToAND = dataLines[selectedDataLine].input;
            }
        } else if (selectedDataLine >= 5 && selectedDataLine <= 7) {
            // Tape 2 - DA7 is the read/write head  
            if (selectedDataLine == 6) {
                valueToAND = tape2.read();
            } else {
                valueToAND = dataLines[selectedDataLine].input;
            }
        } else {
            valueToAND = dataLines[selectedDataLine].input;
        }
    } else {
        // Normal operation - use input line or memory
        if (selectedDataLine < 2) {
            valueToAND = readMemoryCell(selectedDataLine);
        } else {
            valueToAND = dataLines[selectedDataLine].input;
        }
    }
    
    registerValue = registerValue & valueToAND;
}

void Emulator::executeLD() {
    if (tapeMode && (selectedDataLine >= 2 && selectedDataLine <= 7)) {
        handleTapeOperation(selectedDataLine, false);
    } else {
        // Normal memory operation for DA1/DA2, input for others
        if (selectedDataLine < 2) {
            registerValue = readMemoryCell(selectedDataLine);
        } else {
            registerValue = dataLines[selectedDataLine].input;
        }
    }
}

void Emulator::executeOR() {
    bool valueToOR;
    if (tapeMode && (selectedDataLine >= 2 && selectedDataLine <= 7)) {
        // For tape operations, we need to read the current tape value
        if (selectedDataLine >= 2 && selectedDataLine <= 4) {
            // Tape 1 - DA4 is the read/write head
            if (selectedDataLine == 3) {
                valueToOR = tape1.read();
            } else {
                valueToOR = dataLines[selectedDataLine].input;
            }
        } else if (selectedDataLine >= 5 && selectedDataLine <= 7) {
            // Tape 2 - DA7 is the read/write head  
            if (selectedDataLine == 6) {
                valueToOR = tape2.read();
            } else {
                valueToOR = dataLines[selectedDataLine].input;
            }
        } else {
            valueToOR = dataLines[selectedDataLine].input;
        }
    } else {
        // Normal operation - use input line or memory
        if (selectedDataLine < 2) {
            valueToOR = readMemoryCell(selectedDataLine);
        } else {
            valueToOR = dataLines[selectedDataLine].input;
        }
    }
    
    registerValue = registerValue | valueToOR;
}

void Emulator::executeOUT() {
    outputFlag = true;
}

void Emulator::executeNOT() {
    registerValue = !registerValue;
}

void Emulator::executeDataSelect(int dataLineIndex) {
    selectedDataLine = dataLineIndex;
}

std::string Emulator::getInstructionName(const std::string& instruction) const {
    return instruction;
}

bool Emulator::readMemoryCell(int dataLineIndex) {
    if (dataLineIndex < 2) {
        return dataLines[dataLineIndex].memoryValue;
    }
    return dataLines[dataLineIndex].input;
}

void Emulator::writeMemoryCell(int dataLineIndex, bool value) {
    if (dataLineIndex < 2) {
        dataLines[dataLineIndex].memoryValue = value;
    } else {
        dataLines[dataLineIndex].output = value;
    }
}

void Emulator::handleTapeOperation(int dataLineIndex, bool isWrite) {
    if (!tapeMode) return;
    
    // Tape 1: DA3 (left), DA4 (read/write), DA5 (right)
    // Tape 2: DA6 (left), DA7 (read/write), DA8 (right)
    
    if (dataLineIndex >= 2 && dataLineIndex <= 4) {
        // Tape 1 operations
        if (dataLineIndex == 2 && registerValue) { // DA3 - move left
            tape1.moveLeft();
        } else if (dataLineIndex == 3) { // DA4 - read/write
            if (isWrite) {
                tape1.write(registerValue);
            } else {
                registerValue = tape1.read();
            }
        } else if (dataLineIndex == 4 && registerValue) { // DA5 - move right
            tape1.moveRight();
        }
    } else if (dataLineIndex >= 5 && dataLineIndex <= 7) {
        // Tape 2 operations
        if (dataLineIndex == 5) { // DA6 - move left
            tape2.moveLeft();
        } else if (dataLineIndex == 6 && registerValue) { // DA7 - read/write
            if (isWrite) {
                tape2.write(registerValue);
            } else {
                registerValue = tape2.read();
            }
        } else if (dataLineIndex == 7 && registerValue) { // DA8 - move right
            tape2.moveRight();
        }
    }
}