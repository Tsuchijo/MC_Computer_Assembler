# Minecraft Computer Assembly

A comprehensive C++ assembler and emulator that converts assembly code into Minecraft music disc sequences for use in redstone computers. This tool maps assembly opcodes to Minecraft music discs, generates give commands for shulker boxes, and provides a full emulation environment for testing programs.

## Overview

This project provides a complete development environment for programming Minecraft redstone computers. It transforms custom assembly language into sequences of Minecraft music discs and includes an emulator for testing and debugging programs before deployment.

Each instruction is encoded by a music disc, with instructions 1-12 being farmable in vanilla survival Minecraft, while 13-15 are non-renewable. Only the first 7 discs encode instructions for the computer, while the remaining 8 are used to index the databus.

## Computer Architecture

The computer operates with a register-based architecture featuring:
- **Single Register**: All operations use a central 1-bit register for computation
- **8 Data Lines**: Each data line (DA1-DA8) can function as memory or I/O depending on configuration
- **HALT Flag**: Program control flag that prevents looping when set
- **SKIP Flag**: Instruction control flag that causes SKZ to skip the next instruction
- **Dual Tape Memory**: Two infinite tape memories with read/write heads for Turing completeness
- **Program Counter**: Controls program flow with conditional skipping

Instructions execute at 12 game ticks per cycle with a master clock. The instruction memory uses shulker boxes filled with music discs read by jukeboxes.

## Features

### Assembler Features
- **Macro Support**: Define and use macros with parameters for code reuse
- **Recursive Macros**: Support for nested macro calls with proper parameter substitution
- **SKZ Macro Expansion**: Special expansion when SKZ precedes a macro call - inserts SKZ between each line of the macro
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

| Number | Instruction | Music Disc | Function |
|--------|-------------|------------|----------|
| 1 | NOT | 13 | reg ← NOT(reg) |
| 2 | SKZ | cat | Skip next instruction if SKIP flag is HIGH |
| 3 | OR | blocks | reg ← OR(reg, input) |
| 4 | LD | chirp | reg ← input |
| 5 | XOR | far | reg ← XOR(reg, input) |
| 6 | OUT | mall | output ← reg |
| 7 | AND | mellohi | reg ← AND(reg, input) |
| 8 | DA1 | stal | Select Data Line 1 (HALT Flag in TC mode) |
| 9 | DA2 | strad | Select Data Line 2 (SKIP Flag in TC mode) |
| 10 | DA3 | ward | Select Data Line 3 (Tape #1 Left Shift in TC mode) |
| 11 | DA4 | 11 | Select Data Line 4 (Tape #1 Read/Write in TC mode) |
| 12 | DA5 | wait | Select Data Line 5 (Tape #1 Right Shift in TC mode) |
| 13 | DA6 | pigstep | Select Data Line 6 (Tape #2 Left Shift in TC mode) |
| 14 | DA7 | otherside | Select Data Line 7 (Tape #2 Read/Write in TC mode) |
| 15 | DA8 | 5 | Select Data Line 8 (Tape #2 Right Shift in TC mode) |

### Instruction Details

- **Logic Operations (OR, XOR, AND, LD)**: Use the input value of the selected data line
- **OUT**: Outputs register value to the selected data line output
- **SKZ**: Conditional skip - skips next instruction if SKIP flag (DA2) is HIGH
- **Data Lines**: Can function as memory cells or I/O depending on configuration
- **All values are 1-bit**: Register and data line values are boolean (0 or 1)

### Turing Complete Memory Configuration

In Turing Complete mode, the data lines have specialized functions:

| Data Line | Name | Functionality |
|-----------|------|---------------|
| DA1 | HALT Flag | Read/write memory cell. If HIGH when program ends, HALT instead of loop |
| DA2 | SKIP Flag | Read/write memory cell. If HIGH when SKZ executes, skip next instruction |
| DA3 | Tape #1 Left | OUT shifts tape #1 read/write head left if register value is 1 |
| DA4 | Tape #1 R/W | LD reads tape #1 value, OUT toggles tape #1 value |
| DA5 | Tape #1 Right | OUT shifts tape #1 read/write head right if register value is 1 |
| DA6 | Tape #2 Left | OUT shifts tape #2 read/write head left if register value is 1 |
| DA7 | Tape #2 R/W | LD reads tape #2 value, OUT toggles tape #2 value |
| DA8 | Tape #2 Right | OUT shifts tape #2 read/write head right if register value is 1 |

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
- `-t, --turing` - Enable Turing Complete mode (with tape memory)
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

# Run in Turing Complete mode with tape memory
./build/assembler -e -t program.asm

# Interactive mode with Turing Complete features
./build/assembler -i -t program.asm
```

## Interactive Emulator Mode

The interactive mode provides a powerful debugging environment:

### Commands
- **Enter/step** - Execute next instruction
- **q/quit** - Quit emulator
- **r/run** - Run until completion or halt
- **s/state** - Show current state including flags and tape positions
- **p/program** - Show program with program counter
- **set DAx 0/1** - Set data line x input to 0 or 1
- **tape** - Enable/disable Turing Complete tape mode
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
HALT Flag: false
SKIP Flag: false
Halted: false
Data Lines:
  DA1: IN=0 OUT=0 [HALT Flag] [SELECTED]
  DA2: IN=0 OUT=0 [SKIP Flag]
  DA3: Tape #1 Left Shift
  DA4: Tape #1 R/W (Head: 0, Value: 0)
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

### SKZ Macro Expansion

When `SKZ` immediately precedes a macro call, the assembler performs special expansion by inserting `SKZ` between each line of the macro. This creates a conditional execution pattern where each instruction in the macro is only executed if the previous instruction didn't cause a skip.

**Example:**
```assembly
def test_macro(data_line)
    data_line
    LD
    NOT
    OUT
end

; Normal macro expansion:
test_macro(DA1)
; Expands to: DA1, LD, NOT, OUT

; SKZ macro expansion:
SKZ test_macro(DA1)
; Expands to: DA1, SKZ, LD, SKZ, NOT, SKZ, OUT
```

**Use Cases:**
- **Conditional execution of entire macro**: If the first skip condition fails, all subsequent instructions are skipped
- **Early exit patterns**: Allow macros to exit early based on conditions
- **Complex conditional logic**: Create sophisticated branching behavior within macros

**Important Notes:**
- SKZ is inserted between each line but NOT after the last line
- The pattern only applies when SKZ directly precedes a macro call on the next line
- Regular SKZ behavior is preserved for non-macro instructions
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

#### Conditional Skip Example
```assembly
; Skip next instruction if SKIP flag is set
DA2          ; Select SKIP flag
LD           ; Load SKIP flag value
SKZ          ; Skip next instruction if flag is HIGH
DA1          ; This instruction is skipped if SKIP was HIGH
LD           ; Load from DA1
DA3          ; Select output
OUT          ; Output result
```

#### Turing Complete Tape Operation
```assembly
; Write a value to tape #1 and move right
DA4          ; Select Tape #1 Read/Write
OUT          ; Toggle tape value at current position
DA5          ; Select Tape #1 Right Shift  
OUT          ; Move tape head right
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

## Memory Limitations

Due to the limitations of the memory design, programs must be assembled with the following guarantees:

- **Program Length**: Must be a multiple of 27 instructions
- **Minimum Size**: Must have at least 5 shulker boxes (135 instructions minimum)
- **HALT Flag Timing**: Should not be written less than 2 instructions from program end
- **SKIP Flag Timing**: SKZ cannot be called directly after the SKIP flag has been written to

These limitations ensure proper operation within the Minecraft redstone computer's timing constraints.

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