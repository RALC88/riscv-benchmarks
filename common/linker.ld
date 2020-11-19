/*======================================================================*/
/* Proxy kernel linker for DRAC Project					*/
/*======================================================================*/

/*----------------------------------------------------------------------*/
/* Author:
	Julian Pavon Rivera
*/
/*----------------------------------------------------------------------*/


OUTPUT_ARCH( "riscv" )

/*----------------------------------------------------------------------*/
/* Sections								*/
/*----------------------------------------------------------------------*/

SECTIONS
{
  /* text: Text code section */
  . = 0x100; 						/* We force the system to start at address 0x100 */
  .text : { 
	common/crt.o(.text)
	*(.text) 
	common/crt.o(.ecall)
  }

  /* Memory used to interact with the host dpi functions */
  /*.tohost ALIGN(0x3000) : { *(.tohost) }*/
  .tohost : { *(.tohost) }

  /* Data segment */
  .data ALIGN(0x40) : { *(.data) }

  .sdata : { 
    _gp = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  }

  /* bss segment */
  .sbss : { 
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  }
  .bss : { *(.bss) }

  /* thread-local data segment::start */
  .tdata :
  {
    _tls_data = .;
    common/crt.o(.tdata.begin)
    *(.tdata)
    common/crt.o(.tdata.end)
  }
  .tbss :
  {
    *(.tbss)
    common/crt.o(.tbss.end)
  }
  /* end::extra information to include thread extra information::TODO:continue testing this */

  _end = .;

}
