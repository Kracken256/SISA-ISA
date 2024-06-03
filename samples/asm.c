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

struct program_t {
  uint8_t *data;
  size_t size;
};

bool parse_line(char *str, sisa_inst_t *inst) {
  struct slice_t {
    char *start;
    char *end;
    size_t size;
  };

  const char *mnemonics[] = {"nop", "add", "sub", "bnz", "jmp",
                             "ldb", "stb", "mov", "sys"};
  const char *regs[] = {"pc", "r0", "r1", "r2",  "r3",  "r4",  "r5",  "r6",
                        "r7", "r8", "r9", "r10", "r11", "r12", "r13", "r14"};

  enum state_t {
    STATE_MNEMONIC,
    STATE_REG0,
    STATE_IMM,
    STATE_DONE,
  };

  struct slice_t slice[3];
  enum state_t state = STATE_MNEMONIC;

  while (state != STATE_DONE && *str) {
    switch (state) {
      case STATE_MNEMONIC:
        if (isspace(*str)) {
          str++;
          continue;
        } else {
          slice[0].start = str;
          while (*str && (isalnum(*str) || *str == '.')) {
            str++;
          }
          slice[0].end = str;
          slice[0].size = slice[0].end - slice[0].start;
          state = STATE_REG0;
        }
        break;
      case STATE_REG0:
        if (isspace(*str)) {
          str++;
          continue;
        } else {
          slice[1].start = str;
          while (*str && (isalnum(*str) || *str == '.')) {
            str++;
          }
          slice[1].end = str;
          slice[1].size = slice[1].end - slice[1].start;

          if (*str++ != ',') {
            return false;
          }
          state = STATE_IMM;
        }
        break;
      case STATE_IMM:
        if (isspace(*str)) {
          str++;
          continue;
        } else {
          slice[2].start = str;
          while (*str && (isalnum(*str) || *str == '.')) {
            str++;
          }
          slice[2].end = str;
          slice[2].size = slice[2].end - slice[2].start;
          state = STATE_DONE;
        }
        break;
      case STATE_DONE:
        break;
    }
  }

  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < slice[i].size; j++) {
      slice[i].start[j] = tolower(slice[i].start[j]);
    }
  }

  //   printf("mnemonic: `%.*s`\n", (int)(slice[0].end - slice[0].start),
  //          slice[0].start);
  //   printf("reg0: `%.*s`\n", (int)(slice[1].end - slice[1].start),
  //          slice[1].start);
  //   printf("imm: `%.*s`\n", (int)(slice[2].end - slice[2].start),
  //   slice[2].start);

  for (size_t i = 0; i < sizeof(mnemonics) / sizeof(mnemonics[0]); i++) {
    if (strncmp(mnemonics[i], slice[0].start, slice[0].size) == 0) {
      inst->opcode = i;
      break;
    }
  }

  for (size_t i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
    if (strncmp(regs[i], slice[1].start, slice[1].size) == 0) {
      inst->reg0 = i;
      break;
    }
  }

  for (size_t i = 0; i < sizeof(regs) / sizeof(regs[0]); i++) {
    if (strncmp(regs[i], slice[2].start, slice[2].size) == 0) {
      inst->imm = i;
      return true;
    }
  }

  long num = 0;

  if (slice[2].size > 2 && slice[2].start[0] == '0' &&
      slice[2].start[1] == 'x') {
    num = strtol(slice[2].start, NULL, 16);
    goto check;
  }

  if (slice[2].size > 2 && slice[2].start[0] == '0' &&
      slice[2].start[1] == 'b') {
    num = strtol(slice[2].start, NULL, 2);
    goto check;
  }

  if (slice[2].size > 2 && slice[2].start[0] == '0' &&
      slice[2].start[1] == 'o') {
    num = strtol(slice[2].start, NULL, 8);
    goto check;
  }

  num = atoi(slice[2].start);

check:
  if (num < 0 || num > 255) {
    return false;
  }

  inst->imm = num;

  return true;
}

int main(int argc, char **argv) {
  char line[256];
  size_t row = 0;
  FILE *f, *o;
  sisa_inst_t inst;

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
    if (!parse_line(line, &inst)) {
      fprintf(stderr, "Failed to parse line %zu\n", row);
      return 1;
    }

    fwrite(&inst, sizeof(inst), 1, o);

    row++;
  }

  fclose(f);
  fclose(o);

  /// END: Assemble
  ///=========================================================

  return 0;
}