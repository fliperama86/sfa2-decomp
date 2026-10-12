/* Reconstruction of code that was assembly in the original, written from the
   disassembly one instruction per line. Names and comments are inferred, not original. */
.set noreorder
.set noat
.text

/* BIOS call stub: table B0, function 0x10. The SDK reference has no file for it. */
.globl func_801577ec
.type func_801577ec, @function
func_801577ec:
    addiu $t2, $zero, 0xB0
    jr    $t2
    addiu $t1, $zero, 0x10
    nop
.size func_801577ec, . - func_801577ec
