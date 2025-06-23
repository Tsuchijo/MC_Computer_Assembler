import nbtlib
from nbtlib import Compound, List, String, Byte, Short, Int

def create_music_disc_item(disc_type="music_disc_cat"):
    """Create an NBT item for a music disc"""
    return Compound({
        'id': String(f'minecraft:{disc_type}'),
        'Count': Byte(1)
    })

def create_shulker_box_with_discs(disc_list, color="purple"):
    """Create a shulker box NBT item containing music discs"""
    container_items = List[Compound]()
    
    for i, disc_type in enumerate(disc_list):
        if disc_type:  # Only add if not None/empty
            # Ensure proper minecraft namespace
            if not disc_type.startswith('minecraft:'):
                if not disc_type.startswith('music_disc_'):
                    disc_id = f'minecraft:music_disc_{disc_type}'
                else:
                    disc_id = f'minecraft:{disc_type}'
            else:
                disc_id = disc_type
                
            # Use the format from your working example
            container_item = Compound({
                'item': Compound({
                    'count': Int(1),
                    'id': String(disc_id)
                }),
                'slot': Int(i)
            })
            container_items.append(container_item)
    
    # Create the shulker box item matching your working format
    shulker_box = Compound({
        'id': String(f'minecraft:{color}_shulker_box'),
        'count': Int(1),
        'components': Compound({
            'minecraft:container': container_items
        })
    })
    
    return shulker_box

def create_chest_structure(shulker_data_list):
    """
    Create an NBT structure file for a chest containing shulker boxes
    
    Args:
        shulker_data_list: List of lists, where each inner list contains disc types for one shulker
    """
    
    # Create chest items (shulker boxes)
    chest_items = List[Compound]()
    
    for i, disc_list in enumerate(shulker_data_list):
        if i >= 54:  # Double chest has 54 slots
            break
            
        if disc_list:  # Only create shulker if it has discs
            shulker = create_shulker_box_with_discs(disc_list)
            
            # Map to correct double chest slot
            # Minecraft double chest slots: 0-26 = top chest, 27-53 = bottom chest
            # But visually: top row is slots 0-8, then 9-17, then 18-26 for upper chest
            slot = i
            
            chest_item = Compound({
                'Slot': Byte(slot),
                'count': Int(1),
                'id': String(shulker['id']),
                'components': shulker['components']
            })
            chest_items.append(chest_item)
    
    # Update DataVersion to match your working version
    structure = Compound({
        'DataVersion': Int(4325),  # Updated to your working version
        'size': List[Int]([Int(2), Int(1), Int(1)]),  # 2x1x1 for double chest
        'entities': List[Compound](),
        'blocks': List[Compound]([
            # First chest block (empty in your working example)
            Compound({
                'pos': List[Int]([Int(0), Int(0), Int(0)]),
                'state': Int(0),
                'nbt': Compound({
                    'Items': List[Compound](),  # Empty first chest
                    'id': String('minecraft:chest')
                })
            }),
            # Second chest block (contains all items in your working example)
            Compound({
                'pos': List[Int]([Int(1), Int(0), Int(0)]),
                'state': Int(1),
                'nbt': Compound({
                    'Items': chest_items,  # All items in second chest
                    'id': String('minecraft:chest')
                })
            })
        ]),
        'palette': List[Compound]([
            # Left half of double chest
            Compound({
                'Name': String('minecraft:chest'),
                'Properties': Compound({
                    'waterlogged': String('false'),
                    'facing': String('north'),
                    'type': String('left')
                })
            }),
            # Right half of double chest  
            Compound({
                'Name': String('minecraft:chest'),
                'Properties': Compound({
                    'waterlogged': String('false'),
                    'facing': String('north'),
                    'type': String('right')
                })
            })
        ])
    })
    
    return structure

def load_discs_from_file(input_filename):
    """
    Load disc names from a text file (one disc per line)
    
    Args:
        input_filename: Path to text file containing disc names
        
    Returns:
        List of disc names
    """
    with open(input_filename, 'r') as f:
        discs = [line.strip() for line in f if line.strip()]
    return discs

def chunk_discs_into_shulkers(disc_list, items_per_shulker=27):
    """
    Split a flat list of discs into chunks for shulker boxes
    
    Args:
        disc_list: Flat list of disc names
        items_per_shulker: Number of items per shulker box (default 27)
        
    Returns:
        List of lists, each representing one shulker box
    """
    shulkers = []
    for i in range(0, len(disc_list), items_per_shulker):
        chunk = disc_list[i:i + items_per_shulker]
        # Pad with None to fill empty slots if needed
        while len(chunk) < items_per_shulker:
            chunk.append(None)
        shulkers.append(chunk)
    return shulkers

def save_disc_program_from_file(input_filename, output_filename="disc_program.nbt"):
    """
    Convert a text file of disc names into an NBT structure file
    
    Args:
        input_filename: Path to text file with disc names (one per line)
        output_filename: Output NBT filename
    """
    
    # Load discs from file
    disc_list = load_discs_from_file(input_filename)
    print(f"Loaded {len(disc_list)} discs from {input_filename}")
    
    # Ensure disc names have proper minecraft prefix
    processed_discs = []
    for disc in disc_list:
        if disc.startswith('minecraft:'):
            processed_discs.append(disc)
        elif disc.startswith('music_disc_'):
            processed_discs.append(disc)
        else:
            # Assume it's just the disc name and add prefix
            processed_discs.append(f'music_disc_{disc}')
    
    # Split into shulker box chunks
    shulker_data = chunk_discs_into_shulkers(processed_discs)
    print(f"Split into {len(shulker_data)} shulker boxes")
    
    # Generate structure
    structure = create_chest_structure(shulker_data)
    
    # Save to file with gzip compression (required for structure files)
    nbt_file = nbtlib.File(structure, gzipped=True)
    nbt_file.save(output_filename)
    print(f"Structure saved to {output_filename}")

import argparse
import sys
import os

def main():
    parser = argparse.ArgumentParser(
        description="Convert a text file of music disc names into a Minecraft NBT structure file",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python nbt_generator.py input.txt output.nbt
  python nbt_generator.py my_program.txt -o structures/my_program.nbt
  python nbt_generator.py discs.txt --output my_chest.nbt

The input file should contain one disc name per line, such as:
  13
  cat
  blocks
  chirp
        """
    )
    
    parser.add_argument('input_file', 
                       help='Path to text file containing disc names (one per line)')
    
    parser.add_argument('output_file', nargs='?',
                       help='Output NBT structure file path (optional)')
    
    parser.add_argument('-o', '--output', 
                       help='Output NBT structure file path')
    
    args = parser.parse_args()
    
    # Determine input file
    input_file = args.input_file
    
    # Determine output file
    if args.output:
        output_file = args.output
    elif args.output_file:
        output_file = args.output_file
    else:
        # Default: same name as input but with .nbt extension
        base_name = os.path.splitext(os.path.basename(input_file))[0]
        output_file = f"{base_name}.nbt"
    
    # Check if input file exists
    if not os.path.exists(input_file):
        print(f"Error: Input file '{input_file}' not found", file=sys.stderr)
        sys.exit(1)
    
    try:
        # Convert the file
        save_disc_program_from_file(input_file, output_file)
        
        print(f"\n✓ Successfully converted '{input_file}' to '{output_file}'")
        print("\nTo use in Minecraft:")
        print(f"1. Place '{output_file}' in your world's 'generated/minecraft/structures/' folder")
        print(f"2. Use: /place template minecraft:{os.path.splitext(os.path.basename(output_file))[0]}")
        print("3. Or use a structure block to load and place it")
        
    except Exception as e:
        print(f"Error: Failed to convert file - {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()