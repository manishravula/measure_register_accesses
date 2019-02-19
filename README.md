# Pin module to measure CPU Register accesses 

This pin-tool module helps profile the total number of times CPU(x86 family) registers are accessed (Read/Write/Load/Store) by a binary. 

This includes GP-Registers (RAX,RBX, etc) and their word variants, RIP (instruction pointer register), RFLAGS (flag register for ALU ops) and SIMD registers (XMMs).

# Usage:

l. `cd $PIN_ROOT`, where PIN_ROOT is the root of PIN directory.

l. Copy the regcount.cpp file in `$PIN_ROOT/source/tools/ManualExamples/` inside the pin root directory.

l. `cd $PIN_ROOT/source/tools/ManualExamples/`

l. Add `regcount` to `makefile.rules` in the same directory.

l. run `make` in the same directory.

l. `cd $PIN_ROOT`, use it like any other pintool module. 

l. Example: `./pin ./source/tools/ManualExamples/obj64_intel/regcount.so -- /bin/ls`

The default output is to regcount.txt, but can be changed by using -o flag. 

# Explanation and Debugging:

The module works by inserting an analysis call infront of each instruction that accesses a register. By using the [LEVEL_CORE::INS_RegR](https://software.intel.com/sites/landingpage/pintool/docs/71313/Pin/html/group__INS__BASIC__API__GEN__IA32.html#ga69227a9c571b3899fabdeb51333b36b9) and it's sister API calls, it loops through all read and write registers for each instruction calls and increments counters accordingly. 
