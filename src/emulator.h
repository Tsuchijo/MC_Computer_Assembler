#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
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
    
    bool isRunning() const { return programCounter < instructions.size() && !halted; }
    bool isHalted() const { return halted; }
    int getCurrentPC() const { return programCounter; }
    bool getRegisterValue() const { return registerValue; }
    bool getOutputFlag() const { return outputFlag; }

private:
    struct DataLine {
        bool input = false;
        bool output = false;
    };
    
    bool registerValue;
    DataLine dataLines[8];  // DA1-DA8 (indexed 0-7)
    int selectedDataLine;   // Currently selected data line (0-7)
    int programCounter;
    bool outputFlag;
    bool halted;
    
    std::vector<std::string> instructions;
    std::unordered_map<std::string, int> opcodeToNumber;
    
    void initializeOpcodeMap();
    bool executeInstruction(const std::string& instruction);
    void updateOutput();
    
    void executeXOR();
    void executeJMP();
    void executeAND();
    void executeLD();
    void executeOR();
    void executeOUT();
    void executeNOT();
    void executeDataSelect(int dataLineIndex);
    
    std::string getInstructionName(const std::string& instruction) const;
    void printDataLines() const;
};