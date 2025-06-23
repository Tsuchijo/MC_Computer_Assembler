def HIGH()
    LD
    XOR
    NOT
end

def set_cell_values(one_val, two_val, three_val, four_val, LEFT, WRITE, RIGHT)
    WRITE
    one_val()
    XOR
    OUT
end

def jump_to(one_val, two_val, three_val, four_val, LEFT, WRITE, RIGHT)
    set_cell_values(one_val, two_val, three_val, four_val, LEFT, WRITE, RIGHT)
    HIGH()
    DA1
    OUT
    NOT
    NOT
end

SKZ
jump_to(HIGH, HIGH, HIGH, HIGH, DA6, DA7, DA8)