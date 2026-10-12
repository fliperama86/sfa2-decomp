/* Reconstruction of code that was assembly in the original, written from the
   disassembly one instruction per line. Names and comments are inferred, not original. */
.set noreorder
.set noat
.text

/* BIOS call stub: table B0, function 0x13. The SDK reference has no file for it. */
.globl func_801577dc
.type func_801577dc, @function
func_801577dc:
    addiu $t2, $zero, 0xB0
    jr    $t2
    addiu $t1, $zero, 0x13
    nop
.size func_801577dc, . - func_801577dc
