#include <catch2/catch_test_macros.hpp>
#include "assembler.h"
#include <fstream>
#include <string>

std::string getTestFilePath(const std::string& filename) {
    // When running from build directory, test files are in ../../tests/
    return "../../tests/" + filename;
}

TEST_CASE("Assembler can parse basic opcodes", "[assembler]") {
    
    SECTION("Single opcode file") {
        std::string testFile = getTestFilePath("test_single_opcode.asm");
        
        Assembler assembler(testFile);
        auto instructions = assembler.getInstructions();
        REQUIRE(instructions.size() == 3);
        REQUIRE(instructions[0] == "XOR");
        REQUIRE(instructions[1] == "AND");
        REQUIRE(instructions[2] == "JMP");
    }
}

TEST_CASE("Macro definitions are parsed correctly", "[assembler][macro]") {
    
    SECTION("Multiple consecutive macros") {
        std::string testFile = getTestFilePath("test_multiple_macros.asm");
        
        Assembler assembler(testFile);
        
        // You might need to add: int getMacroCount() const { return macroTable.size(); }
        REQUIRE(assembler.getMacroCount() == 2);
        
        auto instructions = assembler.getInstructions();
        REQUIRE(instructions.size() == 2);
    }
}

TEST_CASE("Comment removal works correctly", "[assembler][utilities]") {
    Assembler assembler;  // Using default constructor
    
    std::string line1 = "XOR ; this is a comment";
    assembler.removeComments(line1);
    assembler.trimWhitespace(line1);
    REQUIRE(line1 == "XOR");
    
    std::string line2 = "; full line comment";
    assembler.removeComments(line2);
    assembler.trimWhitespace(line2);
    REQUIRE(line2 == "");
}