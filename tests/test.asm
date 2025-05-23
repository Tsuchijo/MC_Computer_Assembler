; Basic Test Code, Wait for input on Data 8 then execute 2 bit addition

def abc(a,b,c)
    a
    b
    c
end

def unused()
    JMP
end

DA8
LD 
JMP
DA3
LD
DA1
XOR
OUT 
NOT
AND 
DA4
XOR
DA2
XOR 
OUT 
abc(JMP, JMP, JMP)
