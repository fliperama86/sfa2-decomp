/* SPDX-License-Identifier: MIT */
/* libcard/card.c of the SDK reference: function _card_clear, written by hand in assembly. An explicit assembly
   exception, not a function that could not be read: the reference's C for it is

       long _card_clear(long chan) {
           _new_card();
           return _card_write(chan, 0x3F, 0);
       }

   The original keeps `chan` in its argument slot of the caller's frame across the first call, where every
   compiler tried keeps it in a saved register (4 or 8 bytes more). With the parameter's address taken, GCC 2.5.7 gives
   these twelve instructions except in four places: the return address at 0x10($sp) where the original has
   0x14($sp), and the stack's restore in the return's delay slot where the original has it before the return.
   No source text and no option of the compilers tried gives the original's four. Inferred from those builds;
   which tools made the object is not known. */
.set noreorder
.set noat
.text
.globl _card_clear
.type _card_clear, @function
_card_clear:
    addiu $sp, $sp, -0x18
    sw    $ra, 0x14($sp)
    jal   _new_card
    sw    $a0, 0x18($sp)
    lw    $a0, 0x18($sp)
    addiu $a1, $zero, 0x3F
    jal   _card_write
    addu  $a2, $zero, $zero
    lw    $ra, 0x14($sp)
    addiu $sp, $sp, 0x18
    jr    $ra
    nop
.size _card_clear, . - _card_clear
