/* Reconstruction of code that was assembly in the original, written from the
   disassembly one instruction per line. Names and comments are inferred, not original. */
.set noreorder
.set noat
.text

/* BIOS call stub: table B0, function 0x12. The SDK reference has no file for it. */
.globl func_8015762c
.type func_8015762c, @function
func_8015762c:
    addiu $t2, $zero, 0xB0
    jr    $t2
    addiu $t1, $zero, 0x12
    nop
.size func_8015762c, . - func_8015762c
