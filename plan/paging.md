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
