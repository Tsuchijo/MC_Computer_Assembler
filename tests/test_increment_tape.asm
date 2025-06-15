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
    DA1 ; store in DA1
    OUT
    LD
    XOR
    NOT
    LEFT
    OUT;index the tape one to the left

    DA1
    LD 
    WRITE 
    OUT
    LD 
    NOT 
    DA1 ; store in DA1
    AND ; if DA1 is high and the value written is high then store high
    OUT
    LD
    XOR
    NOT
    LEFT
    OUT;index the tape one to the left

    DA1
    LD 
    WRITE 
    OUT
    LD 
    NOT 
    DA1 ; store in DA1
    AND
    OUT
    LD
    XOR
    NOT
    LEFT
    OUT;index the tape one to the left

    DA1
    LD 
    WRITE 
    OUT
    LD 
    NOT 
    DA1 ; store in DA1
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

increment_tape(DA3, DA4, DA5)

DA2 ;Set SKIP flag high
LD;
XOR;
NOT
OUT


increment_tape(DA6, DA7, DA8)

DA1;
LD;
XOR;
NOT
OUT