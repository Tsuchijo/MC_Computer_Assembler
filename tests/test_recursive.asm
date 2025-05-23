def abc(a,b,c)
    a
    b
    c
end

def abcpermute(a,b,c)
    abc(a, b, c)
    abc(b, c, a)
    abc(c, a, b)
end

abcpermute(JMP, XOR, AND)