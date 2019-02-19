# Pin module to measure CPU Register accesses 

This pin-tool module helps profile the total number of times CPU(x86 family) registers are accessed (Read/Write/Load/Store) by a binary. 

This includes GP-Registers (RAX,RBX, etc) and their word variants, RIP (instruction pointer register), RFLAGS (flag register for ALU ops) and SIMD registers (XMMs).

# Usage:

l. `cd $PIN_ROOT`, where PIN_ROOT is the root of PIN directory.

l. Copy the regcount.cpp file in `$PIN_ROOT/source/tools/ManualExamples/` inside the pin root directory.

l. `cd $PIN_ROOT/source/tools/ManualExamples/`

l. Add `regcount` to line 20(TEST_TOOL_ROOTS) in `makefile.rules` in the same directory.

l. run `make` in the same directory.

l. `cd $PIN_ROOT`, use it like any other pintool module. 

l. Example: `./pin ./source/tools/ManualExamples/obj64_intel/regcount.so -- /bin/ls`

The default output is to regcount.txt, but can be changed by using -o flag. 

# Explanation and Debugging:

The module works by inserting an analysis call infront of each instruction that accesses a register. By using the [LEVEL_CORE::INS_RegR](https://software.intel.com/sites/landingpage/pintool/docs/71313/Pin/html/group__INS__BASIC__API__GEN__IA32.html#ga69227a9c571b3899fabdeb51333b36b9) and it's sister API calls, it loops through all read and write registers for each instruction calls and increments counters accordingly. 

For debugging which register was identified as accessed, change the `C_DEBUG` flag inside regcount.cpp to 1. After running this new pin-module, the `regcount.out` file contains detailed information about the analysis. An example of such trace `example/regcount.out` for a simple multiplication program test.s is included in the repo.

You can compile the `example/test.s` file by running the following commands.

```BASH
nasm -f elf64 -o test.o test.s
ld -o test test.o
```

A sample line in the debug output is illustrated here:

```ASSEMBLY
mov eax, 0x1 -- (WRITE):0 -- eax : 0
```
The line explains that the instruction under analysis is `mov eax, 0x1` and that it was identified as making a `WRITE` access to a register. The total count of write accesses is printed immedeately afterward as `WRITE:0` meaning this was the first(zero-indexed) write to be performed by the whole binary. Then, the register that the write has been performed to is printed as `eax`, followed by it's location in the write-queue.






