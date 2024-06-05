/**
 * Simple Instruction Set Architecture (SISA) library
 * Author: Wesley Jones
 * Date: 2/6/2024
 * License: Unlicense (Public Domain)
 */

#ifndef __SISA_SISA_H__
#define __SISA_SISA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

typedef struct sisa_mem_t {
  uint8_t *base;
  uint64_t size;
} sisa_mem_t;

typedef enum sisa_reg_t {
  SISA_PC = 0,   /* Program Counter */
  SISA_X0 = 1,   /* Zero Register */
  SISA_X1 = 2,   /* GPR 2 */
  SISA_X2 = 3,   /* GPR 3 */
  SISA_X3 = 4,   /* GPR 4 */
  SISA_X4 = 5,   /* GPR 5 */
  SISA_X5 = 6,   /* GPR 6 */
  SISA_X6 = 7,   /* GPR 7 */
  SISA_X7 = 8,   /* GPR 8 */
  SISA_X8 = 9,   /* GPR 9 */
  SISA_X9 = 10,  /* GPR 10 */
  SISA_X10 = 11, /* GPR 11 */
  SISA_X11 = 12, /* GPR 12 */
  SISA_X12 = 13, /* GPR 13 */
  SISA_X13 = 14, /* GPR 14 */
  SISA_X14 = 15, /* GPR 15 */
} sisa_reg_t;

#define SISA_NUMREGS 16

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

typedef struct sisa_insta_t {
  sisa_opcode_t opcode : 5;
  sisa_reg_t rd0 : 4;
  sisa_reg_t rd1 : 4;
  uint32_t imm : 11;
} __attribute__((packed)) sisa_insta_t;

typedef union sisa_inst_t {
  sisa_insta_t a;
  uint8_t raw[15];
} __attribute__((packed)) sisa_inst_t;

#define SISA_INST_SIZE sizeof(sisa_inst_t)

#define SISA_MEMORY_ASSIGN(_mem, _addr, _size) \
  {                                            \
    (_mem).base = _addr;                       \
    (_mem).size = _size;                       \
  }

#define SISA_PROGRAM_LOAD(_mem, _program, _size) \
  {                                              \
    for (uint64_t i = 0; i < _size; i++) {       \
      (_mem).base[i] = _program[i];              \
    }                                            \
  }

#define SISA_SYSCALL(_cpu, _num, _func) \
  { (_cpu).tab[_num] = _func; }

#define SISA_SYS_MAX 512

#define SISA_ASM0(_opcode, _reg0, _reg1, _imm) \
  (((_opcode) << 27) | ((_reg0) << 23) | ((_reg1) << 19) | (_imm))

#ifdef __cplusplus
}
#endif

#endif  // __SISA_SISA_H__
