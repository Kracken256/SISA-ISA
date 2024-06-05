1. Pseudo-random Opcode Masking for code obfuscation
    - Use the XSalsa20 stream cipher to generate a pseudo-random mask for the opcode field of each instruction.
    - XOR the opcode field with the generated mask to decrypt the instruction.
    - Implement extended instructions to interact with the decryptor (change the nonce, key, etc.).
1. Port the LLVM RISCV backend to the SISA architecture
    - Modify the RISCV backend to emit SISA instructions instead of RISCV instructions.
    - Implement a SISA target machine in the LLVM backend.
    - Use the LLVM toolchain to compile C code to SISA assembly.
1. Start developing malware in SISA
    - Malware can not be disassmbled without the key.
    - Malware can be embedded via the emulator several levels deep. Making manual analysis virtually impossible.
    - Use High Level C code to generate complex malware.
    - Use malware for internal purposes.
1. Design and standardize the extention instructions
    - Implement a set of instructions that can be used to extend the functionality of the SISA architecture.
    - Define a standard for implementing extension instructions.
    - Implement a set of extension instructions for various operations.
1. Write a bunch of test cases and fuzzers
    - Write a test suite that covers all instructions and edge cases.
    - Write a fuzzer that generates random SISA instructions / data.
    - Emulate a pseudo-random bitstream and verify it does not crash.
