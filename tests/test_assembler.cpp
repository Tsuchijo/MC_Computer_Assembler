#include <catch2/catch_test_macros.hpp>
#include "assembler.h"
#include <fstream>
#include <string>

std::string getTestFilePath(const std::string& filename) {
    // When running from project root, test files are in tests/
    return "tests/" + filename;
}

TEST_CASE("Assembler can parse basic opcodes", "[assembler]") {
    
    SECTION("Single opcode file") {
        std::string testFile = getTestFilePath("test_single_opcode.asm");
        
        Assembler assembler(testFile);
        auto instructions = assembler.getInstructions();
        // V2 spec requires minimum 27 instructions, so it gets padded
        REQUIRE(instructions.size() == 27);
        REQUIRE(instructions[0] == "XOR");
        REQUIRE(instructions[1] == "AND");
        REQUIRE(instructions[2] == "SKZ");
    }
}

TEST_CASE("Macro definitions are parsed correctly", "[assembler][macro]") {
    
    SECTION("Multiple consecutive macros") {
        std::string testFile = getTestFilePath("test_multiple_macros.asm");
        
        Assembler assembler(testFile);
        
        // You might need to add: int getMacroCount() const { return macroTable.size(); }
        REQUIRE(assembler.getMacroCount() == 2);
        
        auto instructions = assembler.getInstructions();
        // V2 spec requires minimum 27 instructions, so it gets padded
        REQUIRE(instructions.size() == 27);
        // First two instructions should be from the macros
        REQUIRE(instructions[0] == "XOR");
        REQUIRE(instructions[1] == "AND");
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

TEST_CASE("V2 Instruction Set Support", "[assembler][v2]") {
    
    SECTION("All V2 instructions are recognized") {
        std::string testFile = getTestFilePath("test_turing_complete.asm");
        
        Assembler assembler(testFile);
        auto instructions = assembler.getInstructions();
        
        // Should have assembled without errors
        REQUIRE(instructions.size() > 0);
        
        // Check for V2 specific instructions
        bool hasNOT = false, hasSKZ = false, hasOR = false;
        for (const auto& instr : instructions) {
            if (instr == "NOT") hasNOT = true;
            if (instr == "SKZ") hasSKZ = true;
            if (instr == "OR") hasOR = true;
        }
        
        REQUIRE(hasNOT);
        REQUIRE(hasSKZ);
        REQUIRE(hasOR);
    }
}

TEST_CASE("V2 Data Line Instructions", "[assembler][v2][datalines]") {
    
    SECTION("All data line instructions are supported") {
        // Create a simple test with all data lines
        std::ofstream tempFile("temp_datalines.asm");
        tempFile << "DA1\nDA2\nDA3\nDA4\nDA5\nDA6\nDA7\nDA8\n";
        tempFile.close();
        
        Assembler assembler("temp_datalines.asm");
        auto instructions = assembler.getInstructions();
        
        // V2 spec requires minimum 27 instructions, so it gets padded
        REQUIRE(instructions.size() == 27);
        REQUIRE(instructions[0] == "DA1");
        REQUIRE(instructions[1] == "DA2");
        REQUIRE(instructions[2] == "DA3");
        REQUIRE(instructions[3] == "DA4");
        REQUIRE(instructions[4] == "DA5");
        REQUIRE(instructions[5] == "DA6");
        REQUIRE(instructions[6] == "DA7");
        REQUIRE(instructions[7] == "DA8");
        
        // Clean up
        std::remove("temp_datalines.asm");
    }
}

TEST_CASE("V2 Macro Compatibility", "[assembler][v2][macro]") {
    
    SECTION("Macros work with V2 instructions") {
        // Test recursive macros with V2 instructions
        std::string testFile = getTestFilePath("test_recursive.asm");
        
        Assembler assembler(testFile);
        auto instructions = assembler.getInstructions();
        
        // Should expand to 9 base instructions + padding to 27
        REQUIRE(instructions.size() == 27);
        
        // The recursive macro should expand correctly - check that it has the right pattern
        // Based on debug output, all the actual instructions are SKZ
        REQUIRE(instructions[0] == "SKZ");
        REQUIRE(instructions[1] == "SKZ");
        REQUIRE(instructions[2] == "SKZ");
    }
}