/* Reconstruction of code that was assembly in the original, written from the
   disassembly one instruction per line. Names and comments are inferred, not original. */
.set noreorder
.set noat
.text

/* Returns the global pointer. */
.globl func_8015789c
.type func_8015789c, @function
func_8015789c:
    jr    $ra
    addu  $v0, $gp, $zero
.size func_8015789c, . - func_8015789c
