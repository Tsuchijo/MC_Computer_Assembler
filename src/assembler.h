#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector> 
#include <filesystem>

class Assembler {
    public:
        Assembler(const std::string& inputFile);
        Assembler();
        ~Assembler() = default;
        bool readAssemblyFile(const std::string& inputFile);
        void assemble();
        void writeOutput(const std::string& outputFile);
        void writeOutputCommand(const std::string& outputFile);

    private:
        void parseMacroDefinition(std::vector<std::string>::iterator& currentLine, 
                                  const std::vector<std::string>::iterator& end);
        void parseMacroInvocation(const std::string& line, std::vector<std::string>& instructions);

        // Struct definition goes inside the class if it's only used by this class
        struct MacroDefinition {
            std::string name;
            std::vector<std::string> parameters;  // Formal parameters in the macro definition
            std::vector<std::string> body;        // The macro body lines
        };

        // Member variables
        std::unordered_map<std::string, MacroDefinition> macroTable;

        std::unordered_map<std::string, std::string> opcodeTable;
        std::vector<std::string> discInstructions;
        std::vector<std::string> assemblyFile;

        void removeComments(std::string& line);
        void trimWhitespace(std::string& line);
        bool isValidOpcode(const std::string& opcode);
        bool isValidMacroParameter(const std::string& line, const MacroDefinition& macro);
        bool isValidMacroInvocation(const std::string& line);
        void findParameters(const std::string& line, std::vector<std::string>& parameters);
        const int MAX_NESTED_MACRO_DEPTH = 1024;
};