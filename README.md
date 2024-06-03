# Simple Instruction Set Architecture (ISA)

## Overview

Introducing a uniquely crafted, minimalist instruction set architecture (ISA) that I developed. This Reduced Instruction Set Computing (RISC) architecture boasts a streamlined set of just nine instructions, making it an ideal tool for those diving into the world of computer architecture and assembly language programming from the ground up. The ISA supports machines with varying word sizes, including 8-bit, 16-bit, 32-bit, and 64-bit configurations. Remarkably, the same binary code is compatible across all word size variants, ensuring consistency and simplicity in execution.

## Features

- **Simplicity and Clarity**: With only nine instructions, this ISA is designed to be straightforward and easy to grasp.
- **Versatile Word Size Support**: Compatible with 8-bit, 16-bit, 32-bit, and 64-bit systems, ensuring flexibility and broad applicability.
- **Uniform Binary Execution**: The same binary code runs seamlessly on any machine variant, regardless of the word size, streamlining development and testing processes.

## Instruction Set

| Instruction | Description                               |
|-------------|-------------------------------------------|
| `NOP`       | No operation                              |
| `ADD`       | Unsigned addition                         |
| `SUB`       | Unsigned subtraction                      |
| `BNZ`       | Branch if not zero                        |
| `JMP`       | Jump to address                           |
| `LDB`       | Load byte from memory                     |
| `STB`       | Store byte to memory                      |
| `MOV`       | Move an immediate value to a register     |
| `SYS`       | Execute a system call                     |

This instruction set is not just a learning tool but a stepping stone to understanding the intricacies of computer architecture and the elegance of assembly language programming. Explore and experiment with this simple yet powerful ISA to deepen your knowledge and hone your skills in low-level programming.
