# Minecraft Computer Assembly

A C++ assembler that converts assembly code into Minecraft music disc sequences for use in redstone computers. This tool maps assembly opcodes to Minecraft music discs and generates give commands for shulker boxes containing the program.

## Overview

This assembler transforms custom assembly language into sequences of Minecraft music discs that can be used to program redstone computers. Each opcode corresponds to a specific music disc, allowing complex programs to be represented as collections of music discs in shulker boxes.

## Features

- **Macro Support**: Define and use macros with parameters for code reuse
- **Recursive Macros**: Support for nested macro calls with proper parameter substitution
- **Multiple Output Formats**: Generate both simple disc lists and Minecraft give commands
- **Auto-splitting**: Programs with more than 27 instructions are automatically split into multiple numbered shulker boxes
- **Comment Support**: Use semicolons (`;`) for line comments

## Opcode Mapping

| Opcode | Music Disc | Function |
|--------|------------|----------|
| XOR    | 13         | Exclusive OR operation |
| JMP    | cat        | Jump instruction |
| AND    | blocks     | Logical AND operation |
| LD     | chirp      | Load instruction |
| OR     | far        | Logical OR operation |
| OUT    | mall       | Output instruction |
| NOT    | mellohi    | Logical NOT operation |
| DA1    | stal       | Data Address 1 |
| DA2    | strad      | Data Address 2 |
| DA3    | ward       | Data Address 3 |
| DA4    | 11         | Data Address 4 |
| DA5    | wait       | Data Address 5 |
| DA6    | Pigstep    | Data Address 6 |
| DA7    | otherside  | Data Address 7 |
| DA8    | 5          | Data Address 8 |

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

```bash
./build/assembler <input_file.asm>
```

The assembler generates two output files:
- `output.txt`: Simple list of music disc names
- `outputCommand.txt`: Minecraft give commands for shulker boxes

### Example Assembly Code

```assembly
; Example: 2-bit addition program
DA8          ; Wait for input on Data 8
LD           ; Load value
JMP          ; Jump instruction
DA3          ; Access Data 3
LD           ; Load value
DA1          ; Access Data 1
XOR          ; XOR operation
OUT          ; Output result

; Macro definition
def add_two(a, b)
    a
    LD
    b
    LD
    XOR
end

; Macro usage
add_two(DA1, DA2)
```

## Output Formats

### Simple Output (`output.txt`)
Lists the music disc names in order:
```
5
chirp
cat
ward
...
```

### Minecraft Commands (`outputCommand.txt`)
Generates give commands for shulker boxes:
```
/give @p shulker_box{display:{Name:'{"text":"Program"}'},BlockEntityTag:{Items:[{Slot:0b,id:"minecraft:music_disc_5",Count:1b},{Slot:1b,id:"minecraft:music_disc_chirp",Count:1b}...]}}
```

For programs with more than 27 instructions, multiple shulker boxes are created:
- `Program_Part_1`
- `Program_Part_2`
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
│   ├── assembler.cpp    # Main assembler implementation
│   ├── assembler.h      # Header file
│   └── main.cpp         # Entry point
├── tests/
│   ├── test_assembler.cpp        # Unit tests
│   ├── test.asm                  # Basic test case
│   ├── test_multiple_macros.asm  # Macro test case
│   ├── test_recursive.asm        # Recursive macro test
│   └── test_single_opcode.asm    # Simple opcode test
└── CMakeLists.txt       # Build configuration
```