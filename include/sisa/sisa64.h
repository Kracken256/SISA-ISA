/**
 * Simple Instruction Set Architecture (SISA) library
 * Author: Wesley Jones
 * Date: 2/6/2024
 * License: Unlicense (Public Domain)
 */

#ifndef __SISA_64BIT_H__
#define __SISA_64BIT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sisa/sisa.h>
#include <stdint.h>

struct sisa64_cpu_t;

typedef void (*sisa64_syscall_t)(struct sisa64_cpu_t *m);

typedef struct sisa64_cpu_t {
  sisa_mem_t mem;
  uint64_t reg[SISA_NUMREGS];
  sisa64_syscall_t tab[SISA_SYS_MAX];
  uint8_t halted;
} sisa64_cpu_t;

void sisa64_init(sisa64_cpu_t *m);
void sisa64_reset_reg(sisa64_cpu_t *m);
void sisa64_reset_mem(sisa64_cpu_t *m);
void sisa64_step(sisa64_cpu_t *m);
void sisa64_simulate(sisa64_cpu_t *m);

#ifdef __cplusplus
}
#endif

#endif  // __SISA_64BIT_H__
