; Basic Test Code for V2 - Skip demonstration and basic operations

def abc(a,b,c)
    a
    b
    c
end

def unused()
    SKZ
end

DA8
LD 
DA1        ; Select SKIP flag
LD         ; Load SKIP flag value
SKZ        ; Skip next if SKIP flag is HIGH
DA3
LD
DA1
XOR
OUT 
NOT
AND 
DA4
XOR
DA1
XOR 
OUT 
abc(SKZ, SKZ, SKZ)
