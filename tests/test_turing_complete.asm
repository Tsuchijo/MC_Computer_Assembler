; Test Turing Complete features in V2
; Demonstrates tape operations and flags

; Set up a simple tape operation
DA4        ; Select Tape #1 Read/Write
LD         ; Load current tape value
NOT        ; Invert it
DA4        ; Select Tape #1 Read/Write again
OUT        ; Toggle tape value (write operation)

; Move tape head right
DA5        ; Select Tape #1 Right Shift
OUT        ; Move head right

; Test HALT flag
DA2        ; Select HALT flag
LD         ; Load current HALT flag value
NOT        ; Invert to set HALT
DA2        ; Select HALT flag again
OUT        ; Set HALT flag

; Test SKIP flag operation
DA1        ; Select SKIP flag
LD         ; Load SKIP flag (should be 0)
NOT        ; Set to 1
DA1        ; Select SKIP flag again  
OUT        ; Write SKIP flag
SKZ        ; This should skip next instruction
AND        ; This should be skipped
OR         ; This should execute

; Tape #2 operations
DA6        ; Select Tape #2 Left Shift
OUT        ; Move tape #2 head left
DA7        ; Select Tape #2 Read/Write
OUT        ; Toggle tape #2 value
DA8        ; Select Tape #2 Right Shift
OUT        ; Move tape #2 head right