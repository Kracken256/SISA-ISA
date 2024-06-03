/**
 * Simple Instruction Set Architecture (SISA) library
 * Author: Wesley Jones
 * Date: 2/6/2024
 * License: Unlicense (Public Domain)
 */

#include <sisa/lib.h>
#include <sisa/sisa16.h>
#include <string.h>

uint16_t sisa16_fetch(sisa16_cpu_t *m);
void sisa16_exec(sisa16_cpu_t *m, sisa_inst_t inst);

static void sisa16_syscall_nop(sisa16_cpu_t *m) { m->halted = 1; }

export void sisa16_init(sisa16_cpu_t *m) {
  memset(m, 0, sizeof(sisa16_cpu_t));
  for (size_t i = 0; i < SISA_SYS_MAX; i++) {
    m->tab[i] = sisa16_syscall_nop;
  }
}

export void sisa16_reset_reg(sisa16_cpu_t *m) {
  memset(&m->reg, 0, sizeof(m->reg));
}
export void sisa16_reset_mem(sisa16_cpu_t *m) {
  memset(&m->mem.base, 0, m->mem.size);
}

uint16_t sisa16_fetch(sisa16_cpu_t *m) {
  uint16_t instr;

  if (m->reg[SISA_PC] + 2U > m->mem.size) {
    m->halted = 1;
    return 0;
  }

  instr = m->mem.base[m->reg[SISA_PC]++];
  instr |= m->mem.base[m->reg[SISA_PC]++] << 8;

  return instr;
}

void sisa16_exec(sisa16_cpu_t *m, sisa_inst_t inst) {
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

export void sisa16_step(sisa16_cpu_t *m) {
  sisa16_exec(m, sisa_decode(sisa16_fetch(m)));
}

export void sisa16_simulate(sisa16_cpu_t *m) {
  while (!m->halted) {
    sisa16_step(m);
  }
}