What we got:


Physical memeory allcator. Allocates in 4k frames


What we implement next:
- Paging 

For this, we have to implement the specifics of the RISC-V ISA. We have to be able to access the page tables so that we can add the initial mappings. The kernel itself needs to be identity mapped.
All virtual address above 

I think we go with 48 bit addressing.
That means we have an virtual address space of 48 bit, or 2^^48 byte
We use a higher half kernel. Let's go with this https://en.wikipedia.org/wiki/X86-64#Virtual_address_space_details
This gives us the addresses 0x00000000 00000000- 0x00007FFF FFFFFFFF for the lower half
and the addresses 0xFFFF8000 00000000 - FFFFFFFF FFFFFFFFF for the higher half/kernel


Thus, we map address 0 - heap end to 0xFFFF000 000000000

kmalloc needs to: ( kfree is the inverted op)
- allocate page
- add mapping for the page with correct bits i/e can be accessed from kernel
- allocate memeory on said page

return the memory


start
we boot to 80000000 and set up the page tables that map the kernel 
