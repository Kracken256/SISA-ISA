# Simple Instruction Set Architecture (SISA)

## Overview

Introducing a uniquely crafted, minimalist instruction set architecture (ISA) that I developed. This Reduced Instruction Set Computing (RISC) architecture boasts a streamlined set of just a few instructions, making it an ideal tool for those diving into the world of computer architecture and assembly language programming from the ground up. The ISA supports machines with varying word sizes, including 8-bit, 16-bit, 32-bit, and 64-bit configurations. Remarkably, the same binary code is compatible across all word size variants, ensuring consistency and simplicity in execution.

## Features

- **Simplicity and Clarity**: With only a few instructions, this ISA is designed to be straightforward and easy to grasp.
- **Versatile Word Size Support**: Compatible with 8-bit, 16-bit, 32-bit, and 64-bit systems, ensuring flexibility and broad applicability.
- **Uniform Binary Execution**: The same binary code runs seamlessly on any machine variant, regardless of the word size, streamlining development and testing processes.

## Instruction Set

```c
// sword_t is a signed word size integer type
typedef enum sisa_opcode_t {
  SISA_ADD, /* x[rd0] += x[rd1] + sext(imm) */
  SISA_SUB, /* x[rd0] -= x[rd1] + sext(imm) */
  SISA_AND, /* x[rd0] &= x[rd1] + sext(imm) */
  SISA_ORR, /* x[rd0] |= x[rd1] + sext(imm) */
  SISA_XOR, /* x[rd0] ^= x[rd1] + sext(imm) */
  SISA_SHL, /* x[rd0] <<= x[rd1] + sext(imm) */
  SISA_SHR, /* x[rd0] >>= x[rd1] + sext(imm) */
  SISA_SRA, /* x[rd0] = (sword_t)x[rd0] >> (x[rd1] + sext(imm)) */
  SISA_LDB, /* x[rd0] = cpu.mem[x[rd1] + sext(imm)] */
  SISA_STB, /* cpu.mem[x[rd1] + sext(imm)] = x[rd0] */
  SISA_LDW, /* x[rd0] = cpu.mem[x[rd1] + sext(imm)] */
  SISA_STW, /* cpu.mem[x[rd1] + sext(imm)] = x[rd0] */
  SISA_BEQ, /* if (x[rd0] == x[rd1]) x[PC] += sext(imm) */
  SISA_BNE, /* if (x[rd0] != x[rd1]) x[PC] += sext(imm) */
  SISA_BLT, /* if (x[rd0] < x[rd1]) x[PC] += sext(imm) */
  SISA_BGT, /* if (x[rd0] > x[rd1]) x[PC] += sext(imm) */
  SISA_BLE, /* if (x[rd0] <= x[rd1]) x[PC] += sext(imm) */
  SISA_BGE, /* if (x[rd0] >= x[rd1]) x[PC] += sext(imm) */
  SISA_FBE, /* if (x[rd0] == x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_FBN, /* if (x[rd0] != x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_FBL, /* if (x[rd0] < x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_FBG, /* if (x[rd0] > x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_FLE, /* if (x[rd0] <= x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_FGE, /* if (x[rd0] >= x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_JAL, /* x[rd0] = x[PC] + 3; x[PC] += x[rd1] + sext(imm) */
  SISA_RET, /* x[PC] = (x[rd0] * sext(imm)) + x[rd1] */
  SISA_SYS, /* if (x[rd0] == 0) cpu->tab[x[rd1] + sext(imm)](&cpu) */
  SISA_EXT, /* implementer-defined */
  SISA_CPU, /* see docs */
  SISA_EIF, /* extended instruction format */
  SISA_RES, /* reserved for future use; nop */
  SISA_RE2, /* reserved for future use; nop */
} sisa_opcode_t;
```
```c
typedef struct sisa_inst_t {
  sisa_opcode_t opcode : 5;
  sisa_reg_t rd0 : 4;
  sisa_reg_t rd1 : 4;
  uint32_t imm : 11;
} __attribute__((packed)) sisa_inst_t;
```

This instruction set is not just a learning tool but a stepping stone to understanding the intricacies of computer architecture and the elegance of assembly language programming. Explore and experiment with this simple yet powerful ISA to deepen your knowledge and hone your skills in low-level programming.
