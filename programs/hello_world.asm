; Wesley's Hello World in a Custom CPU Assembly Language

_begin:

xor x7, x7
xor x8, x8
add x7, x0, 0x9f

; store 'H'
add x8, x0, 0x48
stb x8, x7, 0

; store 'e'
xor x8, x8
add x8, x0, 0x65
stb x8, x7, 1

; store 'l'
xor x8, x8
add x8, x0, 0x6c
stb x8, x7, 2

; store 'l'
xor x8, x8
add x8, x0, 0x6c
stb x8, x7, 3

; store 'o'
xor x8, x8
add x8, x0, 0x6f
stb x8, x7, 4

; store ','
xor x8, x8
add x8, x0, 0x2c
stb x8, x7, 5

; store ' '
xor x8, x8
add x8, x0, 0x20
stb x8, x7, 6

; store 'W'
xor x8, x8
add x8, x0, 0x57
stb x8, x7, 7

; store 'o'
xor x8, x8
add x8, x0, 0x6f
stb x8, x7, 8

; store 'r'
xor x8, x8
add x8, x0, 0x72
stb x8, x7, 9

; store 'l'
xor x8, x8
add x8, x0, 0x6c
stb x8, x7, 10

; store 'd'
xor x8, x8
add x8, x0, 0x64
stb x8, x7, 11

; store '!'
xor x8, x8
add x8, x0, 0x21
stb x8, x7, 12

; store '\n'
xor x8, x8
add x8, x0, 0x0a
stb x8, x7, 13

; store '\0'
xor x8, x8
add x8, x0, 0x00
stb x8, x7, 14

; syscall 1
xor x1, x1
add x1, x0, 0x9f
xor x2, x2
add x2, x0, 15
sys x0, x0, 1

beq x0, x0, -165 ; labels not working yet