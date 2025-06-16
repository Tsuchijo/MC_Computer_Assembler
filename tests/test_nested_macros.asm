def HIGH()
    LD
    XOR
    NOT
end

def use_high()
    DA1
    HIGH()
    OUT
end

use_high()