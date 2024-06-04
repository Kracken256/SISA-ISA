/**
 * Simple Instruction Set Architecture (SISA) library
 * Author: Wesley Jones
 * Date: 2/6/2024
 * License: Unlicense (Public Domain)
 */

#include <sisa/lib.h>
#include <sisa/sisa8.h>
#include <string.h>

static void sisa8_syscall_nop(sisa8_cpu_t *m) { m->halted = 1; }

export void sisa8_init(sisa8_cpu_t *m) {
  memset(m, 0, sizeof(sisa8_cpu_t));
  for (size_t i = 0; i < SISA_SYS_MAX; i++) {
    m->tab[i] = sisa8_syscall_nop;
  }
}
static sisa_inst_raw_t sisa8_fetch(sisa8_cpu_t *m) {
  uint32_t instr;

  if (m->reg[SISA_PC] + 3U > m->mem.size || m->reg[SISA_PC] == 0xFC) {
    m->halted = 1;
    return 0;
  }

  instr = m->mem.base[m->reg[SISA_PC]++];
  instr |= m->mem.base[m->reg[SISA_PC]++] << 8;
  instr |= m->mem.base[m->reg[SISA_PC]++] << 16;

  return instr;
}

// signed downcast from 11 bits to 8 bits
#define sext(x) (((x) & 0x400U) ? ((x) | 0x80U) : (x))

typedef int8_t sword_t;

static void sisa8_exec(sisa8_cpu_t *m, sisa_inst_t inst) {
  switch (inst.opcode) {
    case SISA_ADD:
      m->reg[inst.rd0] += m->reg[inst.rd1] + sext(inst.imm);
      break;
    case SISA_SUB:
      m->reg[inst.rd0] -= m->reg[inst.rd1] + sext(inst.imm);
      break;
    case SISA_AND:
      m->reg[inst.rd0] &= m->reg[inst.rd1] + sext(inst.imm);
      break;
    case SISA_ORR:
      m->reg[inst.rd0] |= m->reg[inst.rd1] + sext(inst.imm);
      break;
    case SISA_XOR:
      m->reg[inst.rd0] ^= m->reg[inst.rd1] + sext(inst.imm);
      break;
    case SISA_SHL:
      m->reg[inst.rd0] <<= m->reg[inst.rd1] + sext(inst.imm);
      break;
    case SISA_SHR:
      m->reg[inst.rd0] >>= m->reg[inst.rd1] + sext(inst.imm);
      break;
    case SISA_SRA:
      m->reg[inst.rd0] =
          (sword_t)m->reg[inst.rd0] >> (m->reg[inst.rd1] + sext(inst.imm));
      break;
    case SISA_LDB:
      if (m->reg[inst.rd1] + sext(inst.imm) < m->mem.size) {
        m->reg[inst.rd0] = m->mem.base[m->reg[inst.rd1] + sext(inst.imm)];
      } else {
        m->halted = 1;
      }
      break;
    case SISA_STB:
      if (m->reg[inst.rd1] + sext(inst.imm) < m->mem.size) {
        m->mem.base[m->reg[inst.rd1] + sext(inst.imm)] = m->reg[inst.rd0];
      } else {
        m->halted = 1;
      }
      break;
    case SISA_LDW:
    case SISA_STW:
      m->halted = 1; /* Not supported in 8-bit mode */
      break;
    case SISA_BEQ:
      if (m->reg[inst.rd0] == m->reg[inst.rd1]) {
        m->reg[SISA_PC] += sext(inst.imm);
      }
      break;
    case SISA_BNE:
      if (m->reg[inst.rd0] != m->reg[inst.rd1]) {
        m->reg[SISA_PC] += sext(inst.imm);
      }
      break;
    case SISA_BLT:
      if (m->reg[inst.rd0] < m->reg[inst.rd1]) {
        m->reg[SISA_PC] += sext(inst.imm);
      }
      break;
    case SISA_BGT:
      if (m->reg[inst.rd0] > m->reg[inst.rd1]) {
        m->reg[SISA_PC] += sext(inst.imm);
      }
      break;
    case SISA_BLE:
      if (m->reg[inst.rd0] <= m->reg[inst.rd1]) {
        m->reg[SISA_PC] += sext(inst.imm);
      }
      break;
    case SISA_BGE:
      if (m->reg[inst.rd0] >= m->reg[inst.rd1]) {
        m->reg[SISA_PC] += sext(inst.imm);
      }
      break;
    case SISA_FBE:
      if (m->reg[inst.rd0] == m->reg[inst.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_R4] + sext(inst.imm);
      }
      break;
    case SISA_FBN:
      if (m->reg[inst.rd0] != m->reg[inst.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_R4] + sext(inst.imm);
      }
      break;
    case SISA_FBL:
      if (m->reg[inst.rd0] < m->reg[inst.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_R4] + sext(inst.imm);
      }
      break;
    case SISA_FBG:
      if (m->reg[inst.rd0] > m->reg[inst.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_R4] + sext(inst.imm);
      }
      break;
    case SISA_FLE:
      if (m->reg[inst.rd0] <= m->reg[inst.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_R4] + sext(inst.imm);
      }
      break;
    case SISA_FGE:
      if (m->reg[inst.rd0] >= m->reg[inst.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_R4] + sext(inst.imm);
      }
      break;
    case SISA_JAL:
      m->reg[inst.rd0] = m->reg[SISA_PC] + 3;
      m->reg[SISA_PC] += m->reg[inst.rd1] + sext(inst.imm);
      break;
    case SISA_RET:
      m->reg[SISA_PC] = (m->reg[inst.rd0] * sext(inst.imm)) + m->reg[inst.rd1];
      break;
    case SISA_MUL:
      m->reg[inst.rd0] *= m->reg[inst.rd1] + sext(inst.imm);
      break;
    case SISA_DIV:
      if (m->reg[inst.rd1] + sext(inst.imm) != 0) {
        m->reg[inst.rd0] /= m->reg[inst.rd1] + sext(inst.imm);
      } else {
        // nop
      }
      break;
    case SISA_SYS:
      if (m->reg[inst.rd0] == 0) {
        if (m->reg[inst.rd1] + sext(inst.imm) < SISA_SYS_MAX) {
          m->tab[m->reg[inst.rd1] + sext(inst.imm)](m);
        } else {
          m->halted = 1;
        }
      }
      break;
    case SISA_EXT:
    case SISA_EX2:
      /// TODO: implementer-defined
      break;
  }
}

export void sisa8_step(sisa8_cpu_t *m) {
  sisa8_exec(m, sisa_decode(sisa8_fetch(m)));
}

export void sisa8_simulate(sisa8_cpu_t *m) {
  while (!m->halted) {
    sisa8_step(m);
    m->reg[SISA_R0] = 0;  // R0 is always zero
  }
}