/**
 * Simple Instruction Set Architecture (SISA) library
 * Author: Wesley Jones
 * Date: 2/6/2024
 * License: Unlicense (Public Domain)
 */

#include <assert.h>
#include <ctype.h>
#include <sisa/sisa.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct program_t {
  uint8_t *data;
  size_t size;
};

struct state_t {
  uint64_t offset = 0;
  std::unordered_map<std::string, uint64_t> labels;
};

bool parse_line(state_t &state, char *str, std::optional<sisa_inst_t> &inst) {
  static const std::unordered_map<std::string_view, sisa_opcode_t> mnemonics = {
      {"nop", SISA_DIV}, {"add", SISA_ADD}, {"sub", SISA_SUB},
      {"and", SISA_AND}, {"orr", SISA_ORR}, {"xor", SISA_XOR},
      {"shl", SISA_SHL}, {"shr", SISA_SHR}, {"sra", SISA_SRA},
      {"ldb", SISA_LDB}, {"stb", SISA_STB}, {"ldw", SISA_LDW},
      {"stw", SISA_STW}, {"beq", SISA_BEQ}, {"bne", SISA_BNE},
      {"blt", SISA_BLT}, {"bgt", SISA_BGT}, {"ble", SISA_BLE},
      {"bge", SISA_BGE}, {"fbe", SISA_FBE}, {"fbn", SISA_FBN},
      {"fbl", SISA_FBL}, {"fbg", SISA_FBG}, {"fle", SISA_FLE},
      {"fge", SISA_FGE}, {"jal", SISA_JAL}, {"ret", SISA_RET},
      {"mul", SISA_MUL}, {"div", SISA_DIV}, {"sys", SISA_SYS},
      {"ext", SISA_EXT}, {"ex2", SISA_EX2}};

  static const std::unordered_map<std::string_view, sisa_reg_t> regs = {
      {"pc", SISA_PC},   {"r0", SISA_R0},   {"r1", SISA_R1},
      {"r2", SISA_R2},   {"r3", SISA_R3},   {"r4", SISA_R4},
      {"r5", SISA_R5},   {"r6", SISA_R6},   {"r7", SISA_R7},
      {"r8", SISA_R8},   {"r9", SISA_R9},   {"r10", SISA_R10},
      {"r11", SISA_R11}, {"r12", SISA_R12}, {"r13", SISA_R13},
      {"key", SISA_KEY}};

  static const std::regex op(
      R"(^\s*(\w+)(\s+(\w+)\s*(,\s*(\w+)\s*(,\s*(-?\w+))?)?)?\s*(;.+)?$)");
  static const std::regex label(R"(^\s*(\w+):\s*(;.*)?$)");

  std::cmatch m;
  if (!std::regex_match(str, m, op)) {
    if (!std::regex_match(str, m, label)) {
      return false;
    }

    std::string label(m[1].first, m[1].length());
    state.labels[label] = state.offset;
    return true;
  }

  std::string_view mnemonic(m[1].first, m[1].length());

  if (mnemonic == "ldi") {
    std::cerr << "ldi is not supported" << std::endl;
    return false;
  }

  if (!mnemonics.contains(mnemonic)) {
    std::cerr << "Unknown mnemonic: " << mnemonic << std::endl;
    return false;
  }

  // check first register
  std::string_view reg0 = "r0";
  if (m[3].length() > 0) {
    reg0 = std::string_view(m[3].first, m[3].length());
  }
  if (!regs.contains(reg0)) {
    std::cerr << "Unknown register: " << reg0 << std::endl;
    return false;
  }

  // check second register
  std::string_view reg1 = "r0";
  if (m[5].length() > 0) {
    reg1 = std::string_view(m[5].first, m[5].length());
  }
  if (!regs.contains(reg1)) {
    std::cerr << "Unknown register: " << reg1 << std::endl;
    return false;
  }

  // check immediate
  std::string_view imm_s = "0";
  if (m[7].length() > 0) {
    imm_s = std::string_view(m[7].first, m[7].length());
  }

  uint16_t imm = 0;
  if (imm_s != "0") {
    if (!std::all_of(imm_s.begin(), imm_s.end(), [](char c) {
          return isxdigit(c) || c == '-' || c == 'x' || c == 'b';
        })) {
      if (!state.labels.contains(std::string(imm_s))) {
        std::cerr << "Unknown label: " << imm_s << std::endl;
        return false;
      }

      int64_t x = state.labels[std::string(imm_s)] - (int64_t)state.offset - 3;

      if (x < -0x400 || x > 0x3ff) {
        std::cerr << "Label out of range: " << imm_s << std::endl;
        return false;
      }

      imm = x > 0 ? x : x | 0x800;
    } else {
      bool is_signed = imm_s[0] == '-';
      if (is_signed) {
        imm_s = imm_s.substr(1);
      }

      if (imm_s.starts_with("0x")) {
        imm = std::stoi(std::string(imm_s.substr(2)), nullptr, 16);
      } else if (imm_s.starts_with("0b")) {
        imm = std::stoi(std::string(imm_s.substr(2)), nullptr, 2);
      } else {
        imm = std::stoi(std::string(imm_s), nullptr, 10);
      }

      if (imm > 0x7ff) {
        std::cerr << "Immediate value out of range: " << imm << std::endl;
        return false;
      }

      if (is_signed) {
        imm = ~imm + 1;
      }
    }
  }

  inst = {mnemonics.at(mnemonic), regs.at(reg0), regs.at(reg1), imm};

  return true;
}

int main(int argc, char **argv) {
  char line[256];
  size_t row = 0;
  FILE *f, *o;
  std::optional<sisa_inst_t> inst;
  state_t state;

  ///=========================================================
  /// BEGIN: Assemble
  if (argc < 3) {
    fprintf(stderr, "Usage: %s <assembly> <binary>\n", argv[0]);
    return 1;
  }
  if (!(f = fopen(argv[1], "rb"))) {
    fprintf(stderr, "Failed to open file: %s\n", argv[1]);
    return 1;
  }

  if (!(o = fopen(argv[2], "wb"))) {
    fprintf(stderr, "Failed to open file: %s\n", argv[2]);
    return 1;
  }

  while (fgets(line, sizeof(line), f)) {
    inst = std::nullopt;

    std::string_view sv(line);
    if (sv.empty() || sv[0] == ';' || sv[0] == '\n') {
      continue;
    }
    if (!parse_line(state, line, inst)) {
      fprintf(stderr, "Failed to parse line %zu\n", row);
      return 1;
    }
    row++;

    if (inst.has_value()) {
      fwrite(&*inst, sizeof(*inst), 1, o);
      state.offset += sizeof(*inst);
    }
  }

  fclose(f);
  fclose(o);

  /// END: Assemble
  ///=========================================================

  return 0;
}