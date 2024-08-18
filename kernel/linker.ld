OUTPUT_ARCH( "riscv" )

ENTRY( _start )

MEMORY
{
  ram   (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 2048M 
}


/*FFFF800000000000*/
SECTIONS {
 
  . = 0x80000000;
  /*.boot.data : {
        *(.boot.data)
    }
  .boot.text : {
        *(.boot.text)
    }
    */

  . += 0xFFFF800000000000;
  .text ALIGN (4k): AT (ADDR (.text) - 0xFFFF800000000000) {
    PROVIDE(_kernel_start = .);
    PROVIDE(_text_start = .);
    *(.text.init) *(.text .text.*)
    PROVIDE(_text_end = .);
  } 
  .rodata ALIGN (4k): AT (ADDR (.rodata) - 0xFFFF800000000000) {
    PROVIDE(_rodata_start = .);
    *(.rodata .rodata.*)
    PROVIDE(_rodata_end = .);
  } 

  .data ALIGN (4k): AT (ADDR (.data) - 0xFFFF800000000000) {
    PROVIDE(_data_start = .);
    *(.sdata .sdata.*) *(.data .data.*)
    PROVIDE(_data_end = .);
  } 

  .bss ALIGN (4k) : AT (ADDR (.bss) - 0xFFFF800000000000) {
    PROVIDE(_bss_start = .);
    *(.sbss .sbss.*) *(.bss .bss.*)
    PROVIDE(_bss_end = .);
  } 
    PROVIDE(_uart = 0x10000000);
    PROVIDE(_physical_heap_start = . - 0xFFFF800000000000 );
    PROVIDE(_virtual_heap_start = ORIGIN(ram) + LENGTH(ram) + 0xFFFF800000000000);
    PROVIDE(_virtual_heap_end = 0xFFFFFFFFFFFFFFFF);
    PROVIDE(_kernel_end = _bss_end);
    PROVIDE(_physical_memory_start = ORIGIN(ram));
    PROVIDE(_physical_memory_end = ORIGIN(ram) + LENGTH(ram));
    PROVIDE(_physical_heap_size = _physical_memory_end -_physical_heap_start);
}

