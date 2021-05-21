;/***************************************************************************
; *                                                                         *
; * Copyright (c) 2019 Nuvoton Technology. All rights reserved.             *
; *                                                                         *
; ***************************************************************************/

ENTRY(__vector)

MEMORY
{
  ram  (rwx)   : ORIGIN = 0x80000000, LENGTH = 0x01000000
  dma  (rwx)   : ORIGIN = 0x81000000, LENGTH = 0x00800000
  heap (rwx)   : ORIGIN = 0x81800000, LENGTH = 0x00800000
}

SECTIONS
{
  .text :
  {
    PROVIDE(__image_start = .);
	  PROVIDE(__text_start = .);
  
    PROVIDE(__vectors_start = .);
    *(.vectors);
    . = ALIGN(4);
    PROVIDE(__vectors_end = .);
    *(.init);
    . = ALIGN(4);
    *(.text);
    . = ALIGN(4);
    *(.rodata);
    . = ALIGN(4);
    *(.rodata*);
    . = ALIGN(4);

    etext = .;
    
    PROVIDE(__text_end = .);
  } > ram

  .preinit_array     :
	{
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP (*(.preinit_array*))
		PROVIDE_HIDDEN (__preinit_array_end = .);
	} >ram
	.init_array :
	{
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		PROVIDE_HIDDEN (__init_array_end = .);
	} >ram
	.fini_array :
	{
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP (*(SORT(.fini_array.*)))
		KEEP (*(.fini_array*))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} >ram

  . = ALIGN(4);
  _etext = . ;
  PROVIDE (etext = .);
   
  .data : AT (_etext)
  {
    PROVIDE(__data_start__ = .);
    _data = . ;
    *(.data)
    . = ALIGN(4);
    PROVIDE(__data_end__ = .);
  } > ram

  . = ALIGN(4);
  _edata = . ;
  PROVIDE (edata = .);

  sbss = .;
  .bss :
  {
    PROVIDE (__bss_start__ = .);
    *(.bss)
    *(.bss.**)
    *(COMMON)
    . = ALIGN(4);
    PROVIDE (__bss_end__ = .);
  }>ram  
  ebss = .;
  bss_size = ebss - sbss; 

	.ARM.exidx ALIGN(8) :
	{
		PROVIDE (__exidx_start = .);
		*(.ARM.exidx*)
		PROVIDE (__exidx_end = .);
	} > ram

	.dma ALIGN(8) (NOLOAD) :
	{
		PROVIDE(__dma_start = ORIGIN(dma));
		PROVIDE(__dma_end = ORIGIN(dma) + LENGTH(dma));
	} > dma

	.heap ALIGN(8) (NOLOAD) :
	{
		PROVIDE(__heap_start = ORIGIN(heap));
		end = __heap_start;
		PROVIDE(__heap_end = ORIGIN(heap) + LENGTH(heap));
	} > heap
}

