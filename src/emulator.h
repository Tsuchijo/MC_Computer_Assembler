#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "assembler.h"

class Emulator {
public:
    Emulator();
    ~Emulator() = default;
    
    bool loadProgram(const std::string& assemblyFile);
    void reset();
    bool step();
    void run();
    void runInteractive();
    
    void printState() const;
    void printProgram() const;
    void clearScreen() const;
    void setDataInput(int dataLine, bool value);
    bool getDataOutput(int dataLine) const;
    void enableTapeMode(bool enable) { tapeMode = enable; }
    
    bool isRunning() const { return !halted; }
    bool isHalted() const { return halted; }
    int getCurrentPC() const { return programCounter; }
    bool getRegisterValue() const { return registerValue; }
    bool getOutputFlag() const { return outputFlag; }

private:
    struct DataLine {
        bool input = false;
        bool output = false;
        bool memoryValue = false;  // For DA1/DA2 memory storage
    };
    
    struct TapeMemory {
        std::map<int, bool> tape;  // Infinite tape using map
        int headPosition = 0;
        
        bool read() {
            return tape[headPosition];
        }
        
        void write(bool value) {
            if (value) {
                tape[headPosition] = !tape[headPosition];  // Toggle if high
            }
        }
        
        void moveLeft() { headPosition--; }
        void moveRight() { headPosition++; }
    };
    
    bool registerValue;
    DataLine dataLines[8];  // DA1-DA8 (indexed 0-7)
    int selectedDataLine;   // Currently selected data line (0-7)
    int programCounter;
    bool outputFlag;
    bool halted;
    bool skipNext;
    bool tapeMode;
    
    TapeMemory tape1;
    TapeMemory tape2;
    
    std::vector<std::string> instructions;
    std::unordered_map<std::string, int> opcodeToNumber;
    
    void initializeOpcodeMap();
    bool executeInstruction(const std::string& instruction);
    void updateOutput();
    
    void executeXOR();
    void executeSKZ();
    void executeOR();
    void executeLD();
    void executeAND();
    void executeOUT();
    void executeNOT();
    void executeDataSelect(int dataLineIndex);
    
    // Memory and tape operations
    bool readMemoryCell(int dataLineIndex);
    void writeMemoryCell(int dataLineIndex, bool value);
    void handleTapeOperation(int dataLineIndex, bool isWrite = false);
    
    std::string getInstructionName(const std::string& instruction) const;
    void printDataLines() const;
    void printTapeState() const;
};