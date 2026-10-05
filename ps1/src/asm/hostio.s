/* Reconstruction of code that was assembly in the original, written from the
   disassembly one instruction per line. Names and comments are inferred, not original. */
.set noreorder
.set noat
.text

/* Four calls to a debugging host through the break instruction. Each returns the
   host's result, or -1 when the host reports an error, except the third. */
.globl func_8015fde4
.type func_8015fde4, @function
func_8015fde4:
    addu  $a2, $a1, $zero
    addu  $a1, $a0, $zero
    break 0, 0x103
    beq   $v0, $zero, 1f
    addu  $v0, $v1, $zero
    addiu $v0, $zero, -1
1:
    jr    $ra
    nop
.size func_8015fde4, . - func_8015fde4

.globl func_8015fe04
.type func_8015fe04, @function
func_8015fe04:
    addu  $a3, $a2, $zero
    addu  $a2, $a1, $zero
    addu  $a1, $a0, $zero
    break 0, 0x107
    beq   $v0, $zero, 1f
    addu  $v0, $v1, $zero
    addiu $v0, $zero, -1
1:
    jr    $ra
    nop
.size func_8015fe04, . - func_8015fe04

.globl func_8015fe28
.type func_8015fe28, @function
func_8015fe28:
    addu  $a1, $a0, $zero
    break 0, 0x104
    jr    $ra
    nop
.size func_8015fe28, . - func_8015fe28

.globl func_8015fe38
.type func_8015fe38, @function
func_8015fe38:
    break 0, 0x105
    beq   $v0, $zero, 1f
    addu  $v0, $v1, $zero
    addiu $v0, $zero, -1
1:
    jr    $ra
    nop
.size func_8015fe38, . - func_8015fe38
