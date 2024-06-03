/**
 * Simple Instruction Set Architecture (SISA) library
 * Author: Wesley Jones
 * Date: 2/6/2024
 * License: Unlicense (Public Domain)
 */

#ifndef __SISA_32BIT_H__
#define __SISA_32BIT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sisa/sisa.h>
#include <stdint.h>

struct sisa32_cpu_t;

typedef void (*sisa32_syscall_t)(struct sisa32_cpu_t *m);

typedef struct sisa32_cpu_t {
  sisa_mem_t mem;
  uint32_t reg[SISA_NUMREGS];
  sisa32_syscall_t tab[SISA_SYS_MAX];
  uint8_t halted;
} sisa32_cpu_t;

void sisa32_init(sisa32_cpu_t *m);
void sisa32_reset_reg(sisa32_cpu_t *m);
void sisa32_reset_mem(sisa32_cpu_t *m);
void sisa32_step(sisa32_cpu_t *m);
void sisa32_simulate(sisa32_cpu_t *m);

#ifdef __cplusplus
}
#endif

#endif  // __SISA_32BIT_H__
