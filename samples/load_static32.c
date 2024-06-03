/**
 * Simple Instruction Set Architecture (SISA) library
 * Author: Wesley Jones
 * Date: 2/6/2024
 * License: Unlicense (Public Domain)
 */

#include <assert.h>
#include <sisa/sisa32.h>
#include <stdio.h>
#include <stdlib.h>

void cpu_dump_state(sisa32_cpu_t* cpu) {
  printf("=== CPU STATE ==\n");
  printf("PC:\t%08x\n", cpu->reg[SISA_PC]);
  for (int i = 1; i < SISA_NUMREGS; i++) {
    printf("R%d:\t%08x\n", i - 1, cpu->reg[i]);
  }
  printf("Halted: %d\n", cpu->halted);
  printf("================\n");
}

static void sisa32_syscall_default(sisa32_cpu_t* cpu) {
  (void)cpu;
  printf("Unknown syscall\n");
}

int main(int argc, char** argv) {
  sisa32_cpu_t cpu;
  uint8_t* memory;
  FILE* f;
  size_t program_size;

  ///=========================================================
  /// BEGIN: Load program into memory
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <program rom>\n", argv[0]);
    return 1;
  }
  if (!(f = fopen(argv[1], "rb"))) {
    fprintf(stderr, "Failed to open file: %s\n", argv[1]);
    return 1;
  }
  fseek(f, 0, SEEK_END);
  program_size = ftell(f);
  fseek(f, 0, SEEK_SET);
  if ((memory = (uint8_t*)malloc(program_size)) == NULL) {
    fprintf(stderr, "Failed to allocate memory for program\n");
    return 1;
  }
  if (fread(memory, 1, program_size, f) != program_size) {
    fprintf(stderr, "Failed to read program\n");
    return 1;
  }
  fclose(f);
  /// END: Load program into memory
  ///=========================================================

  ///=========================================================
  /// BEGIN: Do emulation
  sisa32_init(&cpu); /* Init the cpu state */
  SISA_MEMORY_ASSIGN(cpu.mem, memory, program_size);

  for (size_t i = 0; i < SISA_SYS_MAX; i++) {
    SISA_SYSCALL(cpu, i, sisa32_syscall_default);
  }

  cpu_dump_state(&cpu);
  sisa32_simulate(&cpu);
  cpu_dump_state(&cpu);
  free(memory);
  /// END: Do emulation
  ///=========================================================

  return 0;
}