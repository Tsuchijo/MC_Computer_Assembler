# Minecraft Computer Assembly

A comprehensive C++ assembler and emulator that converts assembly code into Minecraft music disc sequences for use in redstone computers. This tool maps assembly opcodes to Minecraft music discs, generates give commands for shulker boxes, and provides a full emulation environment for testing programs.

## Overview

This project provides a complete development environment for programming Minecraft redstone computers. It transforms custom assembly language into sequences of Minecraft music discs and includes an emulator for testing and debugging programs before deployment.

Each instruction is encoded by a music disc, with instructions 1-12 being farmable in vanilla survival Minecraft, while 13-15 are non-renewable. Only the first 7 discs encode instructions for the computer, while the remaining 8 are used to index the databus.

## Computer Architecture

The computer operates with a register-based architecture featuring:
- **Single Register**: All operations use a central 1-bit register for computation
- **8 Data Lines**: Each data line (DA1-DA8) has associated INPUT and OUTPUT lines (1-bit each)
- **OUTPUT Flag**: Controls when register values are output to data lines
- **Program Counter**: Managed by JUMP instructions for program flow control

All instructions execute within the same time frame unless otherwise specified. The JUMP instruction has variable timing depending on its distance from the program start (minimum 2 instructions).

## Features

### Assembler Features
- **Macro Support**: Define and use macros with parameters for code reuse
- **Recursive Macros**: Support for nested macro calls with proper parameter substitution
- **Multiple Output Formats**: Generate numeric lists or Minecraft give commands
- **Custom Output Files**: Specify output filename and format
- **Auto-splitting**: Programs with more than 27 instructions are automatically split into multiple numbered shulker boxes
- **Comment Support**: Use semicolons (`;`) for line comments

### Emulator Features
- **Full Computer Simulation**: Accurate 1-bit register and data line emulation
- **Interactive Mode**: Step-by-step program execution with real-time state display
- **Automatic Mode**: Run programs to completion with execution trace
- **State Inspection**: View register, data lines, program counter, and output flag
- **Data Line Manipulation**: Set input values during execution for testing
- **Clean Terminal Interface**: Non-scrolling display that overwrites previous state

## Instruction Set

| Number | Op Code | Instruction | Music Disc | Function |
|--------|---------|-------------|------------|----------|
| 1 | 0001 | XOR | 13 | Applies XOR gate to register and data, saves to register |
| 2 | 0010 | JMP | cat | Jumps back to start if register is empty (must be ≥2 instructions from start) |
| 3 | 0011 | AND | blocks | Applies AND gate to register and data, saves to register |
| 4 | 0100 | LD | chirp | Loads data into register |
| 5 | 0101 | OR | far | Applies OR gate to register and data, saves to register |
| 6 | 0110 | OUT | mall | Pulses OUTPUT flag |
| 7 | 0111 | NOT | mellohi | Applies NOT gate to register value, saves to register |
| 8 | 1000 | DA1 | stal | Selects Data Line 1 |
| 9 | 1001 | DA2 | strad | Selects Data Line 2 |
| 10 | 1010 | DA3 | ward | Selects Data Line 3 |
| 11 | 1011 | DA4 | 11 | Selects Data Line 4 |
| 12 | 1100 | DA5 | wait | Selects Data Line 5 |
| 13 | 1101 | DA6 | Pigstep | Selects Data Line 6 |
| 14 | 1110 | DA7 | otherside | Selects Data Line 7 |
| 15 | 1111 | DA8 | 5 | Selects Data Line 8 |

### Instruction Details

- **XOR, AND, OR, LD**: Use the INPUT line of the selected data line
- **OUT**: Outputs register value to the OUTPUT line when OUTPUT flag is true
- **JMP**: Variable timing based on distance from program start; must be at least 2 instructions from start
- **DATA Lines**: Each has associated INPUT and OUTPUT lines for bidirectional communication
- **All values are 1-bit**: Register and data line values are boolean (0 or 1)

## Building

### Prerequisites
- CMake 3.10 or higher
- C++ compiler with C++11 support

### Build Instructions

```bash
# Configure the build
cmake -B build

# Build the project
cmake --build build
```

## Usage

### Command Line Options

```bash
./build/assembler [options] <input_file.asm>
```

**Options:**
- `-e, --emulate` - Run in emulator mode
- `-i, --interactive` - Run in interactive emulator mode  
- `-o, --output <file>` - Specify output file (default: output.txt)
- `-m, --minecraft` - Output as Minecraft commands (default: numeric)
- `-h, --help` - Show help message

### Usage Examples

```bash
# Basic assembly (numeric output to output.txt)
./build/assembler program.asm

# Custom output file with numeric format
./build/assembler -o myprogram.txt program.asm

# Minecraft commands to default file
./build/assembler -m program.asm

# Minecraft commands to custom file  
./build/assembler -m -o commands.txt program.asm

# Run program in emulator
./build/assembler -e program.asm

# Interactive debugging mode
./build/assembler -i program.asm
```

## Interactive Emulator Mode

The interactive mode provides a powerful debugging environment:

### Commands
- **Enter/step** - Execute next instruction
- **q/quit** - Quit emulator
- **r/run** - Run until completion
- **s/state** - Show current state
- **p/program** - Show program with program counter
- **set DAx 0/1** - Set data line x input to 0 or 1
- **h/help** - Show command help

### Example Interactive Session
```
Interactive mode commands:
  Enter/step - Execute next instruction
  q/quit     - Quit emulator
  r/run      - Run until completion
  s/state    - Show current state
  p/program  - Show program with PC
  set DAx 0/1- Set data line x input to 0 or 1
  h/help     - Show this help

=== Computer State ===
Program Counter: 0
Register: 0
Selected Data Line: DA1
Output Flag: false
Halted: false
Data Lines:
  DA1: IN=0 OUT=0 [SELECTED]
  DA2: IN=0 OUT=0
  ...

>>> set DA1 1
Set DA1 input to 1

>>> [Enter]
PC:  0 | DA1      | REG:0 | DA1 IN:1 OUT:0
```

## Assembly Language Syntax

### Basic Program Structure
```assembly
; Comments start with semicolon
DA1          ; Select data line 1
LD           ; Load from DA1 input to register
OUT          ; Output register to DA1 output
```

### Macro Definitions
```assembly
; Define a macro with parameters
def add_two(a, b)
    a        ; Select first data line
    LD       ; Load first value
    b        ; Select second data line  
    LD       ; Load second value
    XOR      ; XOR the two values
end

; Use the macro
add_two(DA1, DA2)
OUT          ; Output the result
```

### Program Examples

#### Simple XOR Gate
```assembly
; XOR two inputs and output result
DA1          ; Select input line 1
LD           ; Load first bit
DA2          ; Select input line 2  
XOR          ; XOR with second bit
DA3          ; Select output line
OUT          ; Output result
```

#### Loop Example
```assembly
; Loop until DA1 input is 0
DA1          ; Select DA1
LD           ; Load value
JMP          ; Jump to start if register is 0
DA2          ; Select DA2
LD           ; Load from DA2
DA3          ; Select DA3  
OUT          ; Output to DA3
```

## Output Formats

### Numeric Output
Lists the instruction numbers in order:
```
8
4
2
10
...
```

### Minecraft Commands
Generates give commands for shulker boxes containing music discs:
```
/give @p shulker_box{display:{Name:'{"text":"Program"}'},BlockEntityTag:{Items:[{Slot:0b,id:"minecraft:music_disc_stal",Count:1b},{Slot:1b,id:"minecraft:music_disc_chirp",Count:1b}...]}}
```

For programs with more than 27 instructions, multiple shulker boxes are created:
- `Program_Part_1` (instructions 1-27)
- `Program_Part_2` (instructions 28-54)
- etc.

## Testing

Run the test suite:
```bash
./build/tests/tests
```

Test files are located in the `tests/` directory and include examples of:
- Basic opcodes
- Macro definitions and usage
- Recursive macro calls
- Multiple macro definitions

## Project Structure

```
├── src/
│   ├── assembler.cpp    # Assembler implementation
│   ├── assembler.h      # Assembler header
│   ├── emulator.cpp     # Emulator implementation  
│   ├── emulator.h       # Emulator header
│   └── main.cpp         # Entry point and CLI
├── tests/
│   ├── test_assembler.cpp        # Unit tests
│   ├── test.asm                  # Basic test case
│   ├── test_multiple_macros.asm  # Macro test case
│   ├── test_recursive.asm        # Recursive macro test
│   └── test_single_opcode.asm    # Simple opcode test
└── CMakeLists.txt       # Build configuration
```

## Development

The project uses modern C++ practices and includes:
- Comprehensive error handling
- Unit tests for critical functionality
- Clean separation between assembler and emulator components
- Cross-platform compatibility

## Contributing

When contributing to this project:
1. Follow existing code style and conventions
2. Add unit tests for new features
3. Update documentation for user-facing changes
4. Test both assembler and emulator functionality

## License

This project is designed for educational purposes and Minecraft redstone computer development.