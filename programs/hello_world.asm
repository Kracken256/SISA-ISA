; Wesley's Hello World in a Custom CPU Assembly Language

mov r1, 0xd0
mov r3, 1

; Write 'H'
mov r2, 0x48
stb r1, r2
add r1, r3

; Write 'e'
mov r2, 0x65
stb r1, r2
add r1, r3

; Write 'l'
mov r2, 0x6c
stb r1, r2
add r1, r3

; Write 'l'
mov r2, 0x6c
stb r1, r2
add r1, r3

; Write 'o'
mov r2, 0x6f
stb r1, r2
add r1, r3

; Write ','
mov r2, 0x2c
stb r1, r2
add r1, r3

; Write ' '
mov r2, 0x20
stb r1, r2
add r1, r3

; Write 'W'
mov r2, 0x57
stb r1, r2
add r1, r3

; Write 'o'
mov r2, 0x6f
stb r1, r2
add r1, r3

; Write 'r'
mov r2, 0x72
stb r1, r2
add r1, r3

; Write 'l'
mov r2, 0x6c
stb r1, r2
add r1, r3

; Write 'd'
mov r2, 0x64
stb r1, r2
add r1, r3

; Write '!'
mov r2, 0x21
stb r1, r2
add r1, r3

; Write '\n'
mov r2, 0x0a
stb r1, r2
add r1, r3

; Write system call
mov r1, 0xd0
mov r2, 14
mov r3, 1
sys r3, 0

; Exit system call
mov r3, 0x20
sys r3, 0