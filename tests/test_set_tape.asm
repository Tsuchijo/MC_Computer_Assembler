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
    OR ; append those ors in the middle of the outs in order to buffer them to let the tape cycle
    OUT 
    OR
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

def index(DIRECTION)
    HIGH()
    DIRECTION
    OUT
end

set_cell_values(HIGH, LOW, HIGH, LOW, DA3, DA4, DA5)
set_cell_values(HIGH, LOW, HIGH, LOW, DA6, DA7, DA8)