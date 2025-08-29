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


def IO(DIRECTION)
    HIGH()
    DIRECTION
    OUT
    DIRECTION
    OUT
    DIRECTION
    OUT
    DIRECTION
    OUT
    DIRECTION
    OUT
    DIRECTION
    OUT
    DIRECTION
    OUT
    DIRECTION
end

def sum_values()
    ; Sum two values in the two tapes along with a carry in from the halt flag
    ; sets tape 1 to the output, clears tape 2 and then stores the carry value in the halt flag
    DA1
    LD
    DA7
    XOR
    DA4
    OUT ; sets the first tape to the value

    ; Set the HALT flag to the carry out
    LD
    DA1
    XOR
    DA7
    XOR
    AND
    XOR
    OUT ; Branch 1 of calculation

    DA4
    LD
    DA1
    XOR
    AND
    DA7
    OR
    DA1
    OUT

    ; clear tape 2
    DA7
    HIGH()
    XOR 
    OUT

end

; START OF PROGRAM

; Move the two tapes into position

; Tape 1 left write right: DA6, DA7, DA8
' Tape 2 left write right: DA3, DA4, DA5

IO(DA6) ; Shift the input bits from the input area to working area
IO(DA3)

sum_values()
index(DA6) ; index tape 1 to the left
index(DA3) ; index tape 2 to the left

sum_values()
index(DA6) ; index tape 1 to the left
index(DA3) ; index tape 2 to the left

sum_values()
index(DA6) ; index tape 1 to the left
index(DA3) ; index tape 2 to the left

sum_values()
index(DA6) ; index tape 1 to the left
index(DA3) ; index tape 2 to the left

IO(DA8) ; Shift the input bits from the input area to working area
OUT
DA8
OUT
DA8
OUT
DA8
OUT

IO(DA5) ; Shift the input bits from the input area to working area
OUT
DA5
OUT
DA5
OUT
DA5
OUT

; Set the HALT flag HIGH
HIGH()
DA1
OUT
NOT
NOT