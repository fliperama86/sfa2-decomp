/* SPDX-License-Identifier: MIT */
/* Part of libapi/counter.c of the SDK reference: function StopRCnt, written by hand in assembly. An explicit
   assembly exception, not a function that could not be read: the reference's C for it is

       long StopRCnt(long spec) {
           s32 i = spec & 0xFFFF;
           D_8002D3B4[1] &= ~D_8002D3BC[i];
           return 1;
       }

   and GCC 2.5.7, the compiler that this file's SetRCnt and StartRCnt build with, writes these instructions for it,
   with the two loads of the variables as one line each (`lw $5,D_8002D3B4`, `lw $2,D_8002D3BC($4)`). The
   original's assembler turned those two lines into the five instructions marked below, in this order and with the
   loaded register as the temporary of the second. The assembler step of this build gives them another order and
   $at as the temporary, and no source text was found that changes that. Inferred from the compiler's text and
   the original's instructions; which assembler did it is not known. */
.set noreorder
.set noat
.text
.globl StopRCnt
.type StopRCnt, @function
StopRCnt:
    andi  $a0, $a0, 0xFFFF
    sll   $a0, $a0, 2
    lui   $a1, %hi(D_8002D3B4)          /* the first load, upper half */
    lui   $v0, %hi(D_8002D3BC)          /* the second load, upper half */
    lw    $a1, %lo(D_8002D3B4)($a1)     /* the first load */
    addu  $v0, $v0, $a0                 /* the second load, the index */
    lw    $v0, %lo(D_8002D3BC)($v0)     /* the second load */
    lw    $v1, 4($a1)
    nor   $v0, $zero, $v0
    and   $v0, $v0, $v1
    sw    $v0, 4($a1)
    jr    $ra
    addiu $v0, $zero, 1
.size StopRCnt, . - StopRCnt
