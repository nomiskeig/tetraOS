OUTPUT_ARCH( "riscv" )

ENTRY( _start )

MEMORY
{
  ram   (wxa!ri) : ORIGIN = 0x80000000, LENGTH = 128M
}


/*FFFF800000000000*/
SECTIONS {
 
  . = 0x80000000;
  .boot.data : {
        *(.boot.data)
    }
  . += 0xFFFF800000000000;
  .text ALIGN (4k): AT (ADDR (.text) - 0xFFFF800000000000) {
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
        PROVIDE(_heap_start = . );
  PROVIDE(_memory_start = ORIGIN(ram));
  PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));
    PROVIDE(_heap_size = 0xFFFF800000000000 + _memory_end -_heap_start);
}

