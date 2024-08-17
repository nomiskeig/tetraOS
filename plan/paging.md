# Initial paging setup


We have to map the kernel and the UART thingy.
For the kernel: memory region:



0xFFFF800000000|000 - 8 is 1000 so the first 17 bits are 1, rest 0 
                      | VPN3        | VPN2        | VPN1        | VPN0        | Offset 
bx1111 1111 1111 1111 | 1000 0000 0 | 000 0000 00 | 00 0000 000 | 0 0000 0000 | 0000 0000 0000
0x F

Kernel has additional 0x80000000 off set. 
That is 
111111111111111110000000|0000000010|000000000|000000000|000000000000
the lasat 12 bits are page offsets

p


Kernel start:   0x80000|000 b1000 000 | 0 0000 0000 | 0000 0000 0000
Kernel end:     0x8000e|010 b1000 000 | 0 0000 1110 | 0000 0000 0000
UART: 0x10000000

UART muss von virtual 

bx1111 1111 1111 1111 | 1000 0000 0 | 000 0000 00 | 01 0000 000 | 0 0000 0000 | 0000 0000 0000

# Paging setup in  the kernel

We have a physical memory allocator for physical memory.
Phyiscal memory starts after the kernel code.
We put the tables there

That means:
    - Allocate new physical page
    - Map it


The virtual memory allocator does the following:
- If enough space on already allocated page, return address within page
- If not, allocate new physical frame
- Map a new virtual page to the physical frame
- Return virtual memory address

Mapping:
map_page(virtual, physical)
-> needs write access to physical pages
-> tempory turn of paging?



Recursive mapping:


0xFFFF800000
0x1111 1111 1111 1111 1000 0000 0000... 
-> vpn_3 = 511 = 111111111
0x1111 1111 1111 1111 1111 1111 1100 = 0xFFFFFFC0000...


# NeAw plan

new plan is the following: 
We map the whole physcial address spacke into the virtual address space. After that, virutal addresses are free to use, especially the page tables stay mapped.

In the setup in physicsal memeory, we map the kerenl.
in the kernel itself, we allocate enough page tables for the whole physical address space
The physcial 

We map 128MB + 0x800000000
10000000000000000000000000
128 MB = 10000000000000000000000000 = 0x2000000


Amount of required tables:
ppn_2: 3
ppn_1: for one address: 512 tables 
-> total -> 3 * 512 tables = 1536 tables

In total : 1 + 1 + 3 + 1536 tables = 1357 tables

