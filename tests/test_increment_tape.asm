; Increment the 4 bit value stored in the tape
def increment_tape(LEFT, WRITE, RIGHT)
    ; Assume we are starting at the correct starting index
    WRITE ; select the write value
    LD ; Set value in the register to 1 
    XOR 
    NOT 

    OUT ; Toggle value in the tape
    LD ; LD value
    NOT ; if toggled 1-0 then propagate to next bit
    DA2 ; store in DA1
    OUT
    LD
    XOR
    NOT
    LEFT
    OUT;index the tape one to the left

    DA2
    LD 
    WRITE 
    OUT
    LD 
    NOT 
    DA2 ; store in DA1
    AND ; if DA1 is high and the value written is high then store high
    OUT
    LD
    XOR
    NOT
    LEFT
    OUT;index the tape one to the left

    DA2
    LD 
    WRITE 
    OUT
    LD 
    NOT 
    DA2 ; store in DA1
    AND
    OUT
    LD
    XOR
    NOT
    LEFT
    OUT;index the tape one to the left

    DA2
    LD 
    WRITE 
    OUT
    LD 
    NOT 
    DA2 ; store in DA1
    AND
    OUT

    LD
    XOR
    NOT
    RIGHT
    OUT
    OUT 
    OUT

end

def decrement_tape(LEFT, WRITE, RIGHT)
    ; Assume we are starting at the correct starting index
    WRITE ; select the write value
    LD ; Set value in the register to 1 
    XOR 
    NOT 

    OUT ; Toggle value in the tape
    LD ; LD value
    DA2 ; store in DA1
    OUT
    LD
    XOR
    NOT
    LEFT
    OUT;index the tape one to the left

    DA2
    LD 
    WRITE 
    OUT
    LD 
    DA2 ; store in DA1
    AND ; if DA1 is high and the value written is high then store high
    OUT
    LD
    XOR
    NOT
    LEFT
    OUT;index the tape one to the left

    DA2
    LD 
    WRITE 
    OUT
    LD 
    DA2 ; store in DA1
    AND
    OUT
    LD
    XOR
    NOT
    LEFT
    OUT;index the tape one to the left

    DA2
    LD 
    WRITE 
    OUT
    LD 
    DA2 ; store in DA1
    AND
    OUT

    LD
    XOR
    NOT
    RIGHT
    OUT
    OUT 
    OUT
end

; sets the reg to a high value
def HIGH()
    LD
    XOR
    NOT
end

; sts the reg to a low value
def LOW() 
    LD
    XOR 
end

; indexes tape in the DIRECTION given
def index(DIRECTION)
    HIGH()
    DIRECTION
    OUT
end

; Sets the current cell to a 4bit binary value
def set_cell_values(one_val, two_val, three_val, four_val, LEFT, WRITE, RIGHT)
    WRITE
    one_val()
    XOR
    OUT
    index(LEFT) 

    WRITE
    two_val()
    XOR
    OUT
    index(LEFT) 
        
    WRITE
    three_val()
    XOR
    OUT
    index(LEFT) 
        
    WRITE
    four_val()
    XOR
    OUT
    HIGH()
    RIGHT
    OUT
    OUT
    OUT
end

; checks if the current cell is equal to a value specified, if so leaves register high
def check_equality(one_val, two_val, three_val, four_val, LEFT, WRITE, RIGHT)
    WRITE
    one_val()
    XOR
    DA2 ; store intermediate value in DA1, if any is high it will be high
    OUT

    index(LEFT) 

    WRITE
    two_val()
    XOR
    DA2
    OR
    OUT
    index(LEFT) 
        
    WRITE
    three_val()
    XOR
    DA2
    OR
    OUT
    index(LEFT) 
        
    WRITE
    four_val()
    XOR
    DA2
    OR
    OUT
    HIGH()
    RIGHT
    OUT
    OUT
    OUT

    DA2
    LD
    NOT
end

; sets a point which a jump instruction will branch to, encoded by the 4 bit value
; all this does is set the skip flag low if the value in the selected cell equals the described value
def branch_flag(one_val, two_val, three_val, four_val, LEFT, WRITE, RIGHT)
    check_equality(one_val, two_val, three_val, four_val, LEFT, WRITE, RIGHT)
    NOT
    DA1
    OUT
    NOT
    NOT ; this will set skip flag to low only if the value provided is same as the value in the currently selected cell
end

; jumps to the branch flag encded by the supplied 4 bit value on 
; all this really does is set cell a a value then skip flag high
def jump_to(one_val, two_val, three_val, four_val, LEFT, WRITE, RIGHT)
    set_cell_values(one_val, two_val, three_val, four_val, LEFT, WRITE, RIGHT)
    HIGH()
    DA1
    OUT
    NOT
    NOT
end


; START OF PROGRAM

;At the start set the cell Value in tape 1 to 1010
branch_flag(LOW, LOW, LOW, LOW, DA6, DA7, DA8)
SKZ
set_cell_values(HIGH, LOW, HIGH, LOW, DA3, DA4, DA5)

; jump to the loop (0001)
jump_to(LOW, LOW, LOW, HIGH, DA6, DA7, DA8)

branch_flag(LOW, LOW, LOW, HIGH, DA6, DA7, DA8)

SKZ
decrement_tape(DA3, DA4, DA5) ; decrement in a loop until reaches zero
SKZ
check_equality(LOW, LOW, LOW, LOW, DA3, DA4, DA5)

SKZ
DA1
SKZ
NOT ; if NOT EQUAL then skip the jump to statement 
SKZ

OUT
NOT
NOT

SKZ
jump_to(HIGH, HIGH, HIGH, HIGH, DA6, DA7, DA8) ; if tape equals zero jump to halt



; finally halt program if last branch (1111) 
branch_flag(HIGH, HIGH, HIGH, HIGH, DA6, DA7, DA8)
; set Halt flag low to prevent it from halting prematurely
DA2
LD
XOR 
OUT

SKZ
DA2
SKZ
LD
SKZ
XOR
SKZ
NOT
SKZ
OUT