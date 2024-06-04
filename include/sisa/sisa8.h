/**
 * Simple Instruction Set Architecture (SISA) library
 * Author: Wesley Jones
 * Date: 2/6/2024
 * License: Unlicense (Public Domain)
 */

#ifndef __SISA_8BIT_H__
#define __SISA_8BIT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sisa/sisa.h>
#include <stdbool.h>
#include <stdint.h>

struct sisa8_cpu_t;

typedef void (*sisa8_syscall_t)(struct sisa8_cpu_t *m);

typedef struct sisa8_cpu_t {
  sisa_mem_t mem;
  uint8_t reg[SISA_NUMREGS];
  sisa8_syscall_t tab[SISA_SYS_MAX];
  bool halted;
} sisa8_cpu_t;

void sisa8_init(sisa8_cpu_t *m);
void sisa8_step(sisa8_cpu_t *m);
void sisa8_simulate(sisa8_cpu_t *m);

#ifdef __cplusplus
}
#endif

#endif  // __SISA_8BIT_H__
