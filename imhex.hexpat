#pragma description SISA BITCODE

#pragma MIME application/sisa_bc

import std.core;
import std.sys;
import std.string;
import std.array;

using BitfieldOrder = std::core::BitfieldOrder;

enum sisa_reg_t: u8 {
  SISA_PC = 0,   /* Program Counter */
  SISA_R0 = 1,   /* Zero Register */
  SISA_R1 = 2,   /* GPR 2 */
  SISA_R2 = 3,   /* GPR 3 */
  SISA_R3 = 4,   /* GPR 4 */
  SISA_R4 = 5,   /* GPR 5 */
  SISA_R5 = 6,   /* GPR 6 */
  SISA_R6 = 7,   /* GPR 7 */
  SISA_R7 = 8,   /* GPR 8 */
  SISA_R8 = 9,   /* GPR 9 */
  SISA_R9 = 10,  /* GPR 10 */
  SISA_R10 = 11, /* GPR 11 */
  SISA_R11 = 12, /* GPR 12 */
  SISA_R12 = 13, /* GPR 13 */
  SISA_R13 = 14, /* GPR 14 */
  SISA_KEY = 15, /* Key Register */
};

enum sisa_opcode_t: u8 {
  SISA_ADD = 1,  /* x[rd0] += x[rd1] + sext(imm) */
  SISA_SUB = 2,  /* x[rd0] -= x[rd1] + sext(imm) */
  SISA_AND = 3,  /* x[rd0] &= x[rd1] + sext(imm) */
  SISA_ORR = 4,  /* x[rd0] |= x[rd1] + sext(imm) */
  SISA_XOR = 5,  /* x[rd0] ^= x[rd1] + sext(imm) */
  SISA_SHL = 6,  /* x[rd0] <<= x[rd1] + sext(imm) */
  SISA_SHR = 7,  /* x[rd0] >>= x[rd1] + sext(imm) */
  SISA_SRA = 8,  /* x[rd0] = (sword_t)x[rd0] >> (x[rd1] + sext(imm)) */
  SISA_LDB = 9,  /* x[rd0] = cpu.mem[x[rd1] + sext(imm)] */
  SISA_STB = 10, /* cpu.mem[x[rd1] + sext(imm)] = x[rd0] */
  SISA_LDW = 11, /* x[rd0] = cpu.mem[x[rd1] + sext(imm)] */
  SISA_STW = 12, /* cpu.mem[x[rd1] + sext(imm)] = x[rd0] */
  SISA_BEQ = 13, /* if (x[rd0] == x[rd1]) x[PC] += sext(imm) */
  SISA_BNE = 14, /* if (x[rd0] != x[rd1]) x[PC] += sext(imm) */
  SISA_BLT = 15, /* if (x[rd0] < x[rd1]) x[PC] += sext(imm) */
  SISA_BGT = 16, /* if (x[rd0] > x[rd1]) x[PC] += sext(imm) */
  SISA_BLE = 17, /* if (x[rd0] <= x[rd1]) x[PC] += sext(imm) */
  SISA_BGE = 18, /* if (x[rd0] >= x[rd1]) x[PC] += sext(imm) */
  SISA_FBE = 19, /* if (x[rd0] == x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_FBN = 20, /* if (x[rd0] != x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_FBL = 21, /* if (x[rd0] < x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_FBG = 22, /* if (x[rd0] > x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_FLE = 23, /* if (x[rd0] <= x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_FGE = 24, /* if (x[rd0] >= x[rd1]) x[PC] += x[R4] + sext(imm) */
  SISA_JAL = 25, /* x[rd0] = x[PC] + 3; x[PC] += x[rd1] + sext(imm) */
  SISA_RET = 26, /* x[PC] = (x[rd0] * sext(imm)) + x[rd1] */
  SISA_MUL = 27, /* x[rd0] *= x[rd1] + sext(imm) */
  SISA_DIV = 28, /* x[rd0] /= x[rd1] + sext(imm) */
  SISA_SYS = 29, /* if (x[rd0] == 0) cpu->tab[x[rd1] + sext(imm)](&cpu) */
  SISA_EXT = 30, /* implementer-defined */
  SISA_EX2 = 31, /* implementer-defined */
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