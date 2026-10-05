/* Reconstruction of code that was assembly in the original, written from the
   disassembly one instruction per line. Names and comments are inferred, not original. */
.set noreorder
.set noat
.text

/* BIOS call stub: table B0, function 0x0e. The SDK reference has no file for it. */
.globl func_8015760c
.type func_8015760c, @function
func_8015760c:
    addiu $t2, $zero, 0xB0
    jr    $t2
    addiu $t1, $zero, 0xe
    nop
.size func_8015760c, . - func_8015760c
