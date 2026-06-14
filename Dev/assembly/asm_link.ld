SECTIONS
{
  . = 0x00000000;       /* Text Segment */
  .text : { *(.text) }
  . = 0x40000000;       /* Data Segment */
  .data : { *(.data) }
}


