/**
 * Simple Instruction Set Architecture (SISA) library
 * Author: Wesley Jones
 * Date: 2/6/2024
 * License: Unlicense (Public Domain)
 */

#ifndef __SISA_16BIT_H__
#define __SISA_16BIT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <sisa/sisa.h>
#include <stdint.h>

struct sisa16_cpu_t;

typedef void (*sisa16_syscall_t)(struct sisa16_cpu_t *m);

typedef struct sisa16_cpu_t {
  sisa_mem_t mem;
  uint16_t reg[SISA_NUMREGS];
  sisa16_syscall_t tab[SISA_SYS_MAX];
  uint8_t halted;
} sisa16_cpu_t;

void sisa16_init(sisa16_cpu_t *m);
void sisa16_reset_reg(sisa16_cpu_t *m);
void sisa16_reset_mem(sisa16_cpu_t *m);
void sisa16_step(sisa16_cpu_t *m);
void sisa16_simulate(sisa16_cpu_t *m);

#ifdef __cplusplus
}
#endif

#endif  // __SISA_16BIT_H__
