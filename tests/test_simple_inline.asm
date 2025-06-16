def HIGH()
    LD
    XOR
    NOT
end

def test_macro(val, line)
    line
    val()
    OUT
end

test_macro(HIGH, DA1)