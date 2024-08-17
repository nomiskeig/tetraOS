# Memory layout 
## Physical

0x0 - 0x80000000: Memory mapped io
0x80000000 - 0x100000000: RAM

Four gigabytes total, the last two are RAM.

## Virtual

Kernel address range starts at 0xFFFF800000000000.
The first four gigabytes are mapped to the physical memory.
The start of the virtual heap is therefore 0xFFFF800100000000.
