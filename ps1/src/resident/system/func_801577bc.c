/* Reconstruction of code that was assembly in the original, written from the
   disassembly one instruction per line. Names and comments are inferred, not original. */
.set noreorder
.set noat
.text

/* BIOS call stub: table B0, function 0x0f. The SDK reference has no file for it. */
.globl func_801577bc
.type func_801577bc, @function
func_801577bc:
    addiu $t2, $zero, 0xB0
    jr    $t2
    addiu $t1, $zero, 0xf
    nop
.size func_801577bc, . - func_801577bc
