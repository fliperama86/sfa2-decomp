/* Reconstruction of code that was assembly in the original, written from the
   disassembly one instruction per line. Names and comments are inferred, not original. */
.set noreorder
.set noat
.text

/* Program entry: clears the bss, sets the stack, frame and global pointers, sets up the
   heap and calls the main function. The four words that follow it in the image (the
   memory size table it reads) are not part of this unit. */
.globl func_80118908
.type func_80118908, @function
func_80118908:
    lui   $v0, 0x8018            /* start of the bss, 0x80183904 */
    addiu $v0, $v0, 0x3904
    lui   $v1, 0x801b            /* end of the bss, 0x801ae124 */
    addiu $v1, $v1, -0x1edc
1:
    sw    $zero, 0($v0)
    addiu $v0, $v0, 4
    sltu  $at, $v0, $v1
    bne   $at, $zero, 1b
    nop
    addiu $v0, $zero, 4          /* offset of the entry used in the memory size table */
    nop
    nop
    nop
    nop
    lui   $a0, 0x8012            /* the table right after this routine, 0x801189b4 */
    addiu $a0, $a0, -0x764c
    addu  $a0, $a0, $v0
    lw    $v0, 0($a0)
    lui   $t0, 0x8000
    or    $sp, $v0, $t0          /* stack pointer: top of memory */
    lui   $a0, 0x801b
    addiu $a0, $a0, -0x1edc
    sll   $a0, $a0, 3
    srl   $a0, $a0, 3
    lui   $v1, 0x8018            /* stack size word, 0x80182f64 */
    lw    $v1, 0x2f64($v1)
    nop
    subu  $a1, $v0, $v1
    subu  $a1, $a1, $a0          /* heap size */
    or    $a0, $a0, $t0
    lui   $at, 0x8018
    sw    $ra, 0x3904($at)       /* return address saved in the first bss word */
    lui   $gp, 0x8018
    addiu $gp, $gp, 0x3904
    addu  $fp, $sp, $zero
    jal   InitHeap
    addi  $a0, $a0, 4
    lui   $ra, 0x8018
    lw    $ra, 0x3904($ra)
    nop
    jal   func_801189c4
    nop
    break 0, 1
.size func_80118908, . - func_80118908
