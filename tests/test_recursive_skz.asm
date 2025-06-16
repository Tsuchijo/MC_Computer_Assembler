def inner_macro()
    DA1
    LD
    OUT
end

def outer_macro()
    DA2
    inner_macro()
    DA3
end

SKZ
outer_macro()