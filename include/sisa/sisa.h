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

typedef struct sisa_mem_t {
  uint8_t *base;
  uint64_t size;
} sisa_mem_t;

enum sisa_reg_t {
  SISA_PC = 0,
  SISA_R0,
  SISA_R1,
  SISA_R2,
  SISA_R3,
  SISA_R4,
  SISA_R5,
  SISA_R6,
  SISA_R7,
  SISA_R8,
  SISA_R9,
  SISA_R10,
  SISA_R11,
  SISA_R12,
  SISA_R13,
  SISA_R14,
  SISA_NUMREGS,
};

enum sisa_opcode_t {
  SISA_NOP, /* no operation */
  SISA_ADD, /* cpu.reg[reg0] += cpu.reg[reg1] */
  SISA_SUB, /* cpu.reg[reg0] -= cpu.reg[reg1] */
  SISA_BNZ, /* if (cpu.reg[reg0] != 0) pc += cpu.reg[reg1] */
  SISA_JMP, /* pc += cpu.reg[reg0] + imm */
  SISA_LDB, /* cpu.reg[reg0] = mem[cpu.reg[reg1]] */
  SISA_STB, /* mem[cpu.reg[reg0]] = cpu.reg[reg1] */
  SISA_MOV, /* cpu.reg[reg0] = imm */
  SISA_SYS, /* cpu.systab[reg0](cpu) */
};

typedef struct sisa_inst_t {
  uint8_t opcode : 4;
  uint8_t reg0 : 4;
  uint8_t imm;
} __attribute__((packed)) sisa_inst_t;

static inline sisa_inst_t sisa_decode(uint16_t inst) {
  union {
    uint16_t inst;
    sisa_inst_t sisa;
  } u;

  u.inst = inst;

  return u.sisa;
}

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

#define SISA_SYS_MAX 256

#define SISA_ASM0(_opcode, _reg0, _imm) \
  (((_opcode) << 12) | ((_reg0) << 8) | (_imm))

#ifdef __cplusplus
}
#endif

#endif  // __SISA_SISA_H__
