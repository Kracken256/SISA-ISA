#pragma description SISA BITCODE

#pragma MIME application/sisa_bc

import std.core;
import std.sys;
import std.string;
import std.array;

using BitfieldOrder = std::core::BitfieldOrder;

enum sisa_reg_t: u8 {
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
};

enum sisa_opcode_t: u8 {
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
};

// Compression method & flags
bitfield SISA_INST {
    sisa_opcode_t op: 5;
    sisa_reg_t reg0: 4;
    sisa_reg_t reg1: 4;
    imm: 11;
} [[bitfield_order(BitfieldOrder::LeastToMostSignificant, 24)]];


u64 data_len = std::mem::size();

SISA_INST sisa_bc[data_len/3] @ 0x00;