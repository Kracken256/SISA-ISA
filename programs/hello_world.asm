; Wesley's Hello World in a Custom CPU Assembly Language

add r7, r0, 0x90

; store 'H'
add r8, r0, 0x48
stb r8, r7, 0

; store 'e'
xor r8, r8
add r8, r0, 0x65
stb r8, r7, 1

; store 'l'
xor r8, r8
add r8, r0, 0x6c
stb r8, r7, 2

; store 'l'
xor r8, r8
add r8, r0, 0x6c
stb r8, r7, 3

; store 'o'
xor r8, r8
add r8, r0, 0x6f
stb r8, r7, 4

; store ','
xor r8, r8
add r8, r0, 0x2c
stb r8, r7, 5

; store ' '
xor r8, r8
add r8, r0, 0x20
stb r8, r7, 6

; store 'W'
xor r8, r8
add r8, r0, 0x57
stb r8, r7, 7

; store 'o'
xor r8, r8
add r8, r0, 0x6f
stb r8, r7, 8

; store 'r'
xor r8, r8
add r8, r0, 0x72
stb r8, r7, 9

; store 'l'
xor r8, r8
add r8, r0, 0x6c
stb r8, r7, 10

; store 'd'
xor r8, r8
add r8, r0, 0x64
stb r8, r7, 11

; store '!'
xor r8, r8
add r8, r0, 0x21
stb r8, r7, 12

; store '\n'
xor r8, r8
add r8, r0, 0x0a
stb r8, r7, 13

; store '\0'
xor r8, r8
add r8, r0, 0x00
stb r8, r7, 14

; syscall 1
add r1, r0, 0x90
add r2, r0, 15
sys r0, r0, 1