/**
 * Simple Instruction Set Architecture (SISA) library
 * Author: Wesley Jones
 * Date: 2/6/2024
 * License: Unlicense (Public Domain)
 */

#include <sisa/lib.h>
#include <sisa/sisa64.h>
#include <string.h>

uint16_t sisa64_fetch(sisa64_cpu_t *m);
void sisa64_exec(sisa64_cpu_t *m, sisa_inst_t inst);

static void sisa64_syscall_nop(sisa64_cpu_t *m) { m->halted = 1; }

export void sisa64_init(sisa64_cpu_t *m) {
  memset(m, 0, sizeof(sisa64_cpu_t));
  for (size_t i = 0; i < SISA_SYS_MAX; i++) {
    m->tab[i] = sisa64_syscall_nop;
  }
}

export void sisa64_reset_reg(sisa64_cpu_t *m) {
  memset(&m->reg, 0, sizeof(m->reg));
}
export void sisa64_reset_mem(sisa64_cpu_t *m) {
  memset(&m->mem.base, 0, m->mem.size);
}

uint16_t sisa64_fetch(sisa64_cpu_t *m) {
  uint16_t instr;

  if (m->reg[SISA_PC] + 2U > m->mem.size) {
    m->halted = 1;
    return 0;
  }

  instr = m->mem.base[m->reg[SISA_PC]++];
  instr |= m->mem.base[m->reg[SISA_PC]++] << 8;

  return instr;
}

void sisa64_exec(sisa64_cpu_t *m, sisa_inst_t inst) {
#define reg1 inst.imm & 0xF

  switch (inst.opcode) {
    case SISA_NOP:
      break;
    case SISA_ADD:
      m->reg[inst.reg0] += m->reg[reg1];
      break;
    case SISA_SUB:
      m->reg[inst.reg0] -= m->reg[reg1];
      break;
    case SISA_BNZ:
      if (m->reg[inst.reg0] != 0) m->reg[SISA_PC] += m->reg[reg1];
      break;
    case SISA_JMP:
      m->reg[SISA_PC] += m->reg[inst.reg0] + inst.imm;
      break;
    case SISA_LDB:
      if (m->reg[reg1] >= m->mem.size) {
        m->halted = 1;
        return;
      }
      m->reg[inst.reg0] = m->mem.base[m->reg[reg1]];
      break;
    case SISA_STB:
      if (m->reg[inst.reg0] >= m->mem.size) {
        m->halted = 1;
        return;
      }
      m->mem.base[m->reg[inst.reg0]] = m->reg[reg1];
      break;
    case SISA_MOV:
      m->reg[inst.reg0] = inst.imm;
      break;
    case SISA_SYS:
      m->tab[m->reg[inst.reg0]](m);
      break;
    default:
      // Unknown instruction
      // fail silently
      break;
  }
}

export void sisa64_step(sisa64_cpu_t *m) {
  sisa64_exec(m, sisa_decode(sisa64_fetch(m)));
}

export void sisa64_simulate(sisa64_cpu_t *m) {
  while (!m->halted) {
    sisa64_step(m);
  }
}