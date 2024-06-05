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

static inline bool sisa8_fetch_part(sisa8_cpu_t *m, uint8_t out[3]) {
  if (m->reg[SISA_PC] + 3U > m->mem.size || m->reg[SISA_PC] == (0xFF - 3)) {
    m->halted = 1;
    return false;
  }

  out[0] = m->mem.base[m->reg[SISA_PC]++];
  out[1] = m->mem.base[m->reg[SISA_PC]++];
  out[2] = m->mem.base[m->reg[SISA_PC]++];

  return true;
}

static void sisa8_fetch(sisa8_cpu_t *m, uint8_t out[15]) {
  if (!sisa8_fetch_part(m, out)) return;
  if (((out[0] >> 3) & 0x1F) != SISA_EIF) return;
  if (!sisa8_fetch_part(m, out + 3)) return;
  if (((out[3] >> 3) & 0x1F) != SISA_EIF) return;
  if (!sisa8_fetch_part(m, out + 6)) return;
  if (((out[6] >> 3) & 0x1F) != SISA_EIF) return;
  if (!sisa8_fetch_part(m, out + 9)) return;
  if (((out[9] >> 3) & 0x1F) != SISA_EIF) return;
  if (!sisa8_fetch_part(m, out + 12)) return;
  if (((out[12] >> 3) & 0x1F) != SISA_EIF) return;
}

// signed downcast from 11 bits to 8 bits
#define sext(x) (((x) & 0x400U) ? ((x) | 0x80U) : (x))

typedef int8_t sword_t;

#include <stdio.h>

static inline void sisa8_exec(sisa8_cpu_t *m, sisa_inst_t inst) {
  switch (inst.a.opcode) {
    case SISA_ADD:
      m->reg[inst.a.rd0] += m->reg[inst.a.rd1] + sext(inst.a.imm);
      break;
    case SISA_SUB:
      m->reg[inst.a.rd0] -= m->reg[inst.a.rd1] + sext(inst.a.imm);
      break;
    case SISA_AND:
      m->reg[inst.a.rd0] &= m->reg[inst.a.rd1] + sext(inst.a.imm);
      break;
    case SISA_ORR:
      m->reg[inst.a.rd0] |= m->reg[inst.a.rd1] + sext(inst.a.imm);
      break;
    case SISA_XOR:
      m->reg[inst.a.rd0] ^= m->reg[inst.a.rd1] + sext(inst.a.imm);
      break;
    case SISA_SHL:
      m->reg[inst.a.rd0] <<= m->reg[inst.a.rd1] + sext(inst.a.imm);
      break;
    case SISA_SHR:
      m->reg[inst.a.rd0] >>= m->reg[inst.a.rd1] + sext(inst.a.imm);
      break;
    case SISA_SRA:
      m->reg[inst.a.rd0] = (sword_t)m->reg[inst.a.rd0] >>
                           (m->reg[inst.a.rd1] + sext(inst.a.imm));
      break;
    case SISA_LDB:
      if (m->reg[inst.a.rd1] + sext(inst.a.imm) < m->mem.size) {
        m->reg[inst.a.rd0] = m->mem.base[m->reg[inst.a.rd1] + sext(inst.a.imm)];
      } else {
        m->halted = 1;
      }
      break;
    case SISA_STB:
      if (m->reg[inst.a.rd1] + sext(inst.a.imm) < m->mem.size) {
        m->mem.base[m->reg[inst.a.rd1] + sext(inst.a.imm)] = m->reg[inst.a.rd0];
      } else {
        m->halted = 1;
      }
      break;
    case SISA_LDW:
    case SISA_STW:
      m->halted = 1; /* Not supported in 8-bit mode */
      break;
    case SISA_BEQ:
      if (m->reg[inst.a.rd0] == m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += sext(inst.a.imm);
      }
      break;
    case SISA_BNE:
      if (m->reg[inst.a.rd0] != m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += sext(inst.a.imm);
      }
      break;
    case SISA_BLT:
      if (m->reg[inst.a.rd0] < m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += sext(inst.a.imm);
      }
      break;
    case SISA_BGT:
      if (m->reg[inst.a.rd0] > m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += sext(inst.a.imm);
      }
      break;
    case SISA_BLE:
      if (m->reg[inst.a.rd0] <= m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += sext(inst.a.imm);
      }
      break;
    case SISA_BGE:
      if (m->reg[inst.a.rd0] >= m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += sext(inst.a.imm);
      }
      break;
    case SISA_FBE:
      if (m->reg[inst.a.rd0] == m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_X4] + sext(inst.a.imm);
      }
      break;
    case SISA_FBN:
      if (m->reg[inst.a.rd0] != m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_X4] + sext(inst.a.imm);
      }
      break;
    case SISA_FBL:
      if (m->reg[inst.a.rd0] < m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_X4] + sext(inst.a.imm);
      }
      break;
    case SISA_FBG:
      if (m->reg[inst.a.rd0] > m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_X4] + sext(inst.a.imm);
      }
      break;
    case SISA_FLE:
      if (m->reg[inst.a.rd0] <= m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_X4] + sext(inst.a.imm);
      }
      break;
    case SISA_FGE:
      if (m->reg[inst.a.rd0] >= m->reg[inst.a.rd1]) {
        m->reg[SISA_PC] += m->reg[SISA_X4] + sext(inst.a.imm);
      }
      break;
    case SISA_JAL:
      m->reg[inst.a.rd0] = m->reg[SISA_PC] + 3;
      m->reg[SISA_PC] += m->reg[inst.a.rd1] + sext(inst.a.imm);
      break;
    case SISA_RET:
      m->reg[SISA_PC] =
          (m->reg[inst.a.rd0] * sext(inst.a.imm)) + m->reg[inst.a.rd1];
      break;
    case SISA_SYS:
      if (m->reg[inst.a.rd0] == 0) {
        if (m->reg[inst.a.rd1] + sext(inst.a.imm) < SISA_SYS_MAX) {
          m->tab[m->reg[inst.a.rd1] + sext(inst.a.imm)](m);
        } else {
          m->halted = 1;
        }
      }
      break;
    case SISA_EXT:
      /// TODO: implementer-defined
      // It is a nop for now
      break;
    case SISA_CPU:
      /// TODO: implement
      break;
    default:
      // treat unknown opcodes as nops
      break;
  }
}

export void sisa8_step(sisa8_cpu_t *m) {
  union {
    sisa_inst_t inst;
    uint8_t raw[15];
  } data;

  sisa8_fetch(m, data.raw);
  sisa8_exec(m, data.inst);
  m->reg[SISA_X0] = 0;  // R0 is always zero
}

export void sisa8_simulate(sisa8_cpu_t *m) {
  while (!m->halted) {
    sisa8_step(m);
  }
}