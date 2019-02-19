# Pin module to measure CPU Register accesses (Read/Write/Load/Store) 

This pin-tool module helps profile the total number of times CPU(x86 family) registers are read-from and written-to by a binary. 

This includes GP-Registers (rax,rbx etc) and their word variants, RIP (Instruction Pointer), RFLAGS (Flags for ALU ops) and SIMD registers (xmm-class).

# Usage:

```
Put the regcount.cpp file in ./source/tools/ManualExamples/
```
