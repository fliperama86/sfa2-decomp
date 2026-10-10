/*
 * The port's C for func_80146794. It runs in the PC program in place of the
 * C of the unit s1460ec_r3.c; the PS1 build does not use this file, and the
 * unit keeps the exact C.
 *
 * What is different from the unit's C: the call of func_80120028 is written
 * with an argument, which is either `object` or the word that was at offset
 * 0x18 of the object (the `sequence` pointer) when func_80131094 was entered.
 * The unit calls it with none.
 *
 * Why: func_80120028 reads a0 (`lbu v1,0x8(a0)` at 80120030). On the console
 * a0 holds at that call (jal at 80146808) what the earlier call, func_80131094
 * (jal at 80146800), left in it. The function itself writes no a0 from its
 * entry to the two jals. func_80131094 was read to its end in the listing: it
 * has one return, `jr ra` at 801311a0, and writes a0 at two places only,
 * 801310c4 and 801310f4, both `lw a0,0x18(a1)` (a1 is the object, set by
 * `move a1,a0` at 80131094). Three paths:
 *   1. 801310ac `bne v0,zero,0x801311a0` taken, when the halfword at 0x38 of
 *      the object, lowered by one and cut to 16 bits, is not 0: a0 is the
 *      object.
 *   2. that branch not taken, and 801310bc `bgez v0,0x80131118` taken, when
 *      the signed halfword at 0x3a is not negative: a0 is the object.
 *   3. both not taken: 801310c4 loads a0 from 0x18(a1), 801310f4 loads it
 *      again from the same place (the first store to 0x18(a1) is at 8013113c,
 *      after both loads; 801310f0 stores a byte at 0x80(a1)). So a0 is the
 *      word at 0x18 of the object as it was on entry to func_80131094. That
 *      is why the override reads it before the call. The branch at 801310e4
 *      only skips the second load.
 * Compiled for a PC, the callee would read something else.
 *
 * What the function does (inferred): it moves pos_x by 4 for the field_03
 * values 0x10 and 0x11, raises field_04 when the halfword at 0x3a is
 * negative, calls func_80131094 with the object, then func_80120028.
 *
 * Contract:
 *   Argument: object (a0), a pointer to an object; no result.
 *   Reads: field_03, field_04, the halfword pos_x (0x12), the halfwords at
 *     0x38 and 0x3a, and the word at 0x18. Writes: pos_x, field_04.
 *   Callees: func_80131094 runs as the original code in the test (a leaf; it
 *     reads and writes the object at 0x08, 0x18, 0x38, 0x3a, 0x4a, 0x80, 0x88
 *     and 0x8c, reads three records of the sequence block that the pointer
 *     at 0x18 leads to, and a byte of the frame table that the pointer at
 *     0x8c leads to). The setup builds both blocks. It runs as original code
 *     so that what it leaves in a0 is what the console's code leaves.
 *     func_80120028 (1 argument) is replaced in the test by a recorder that
 *     logs its argument and returns 0. What it does is outside the test. The
 *     test watches the whole object at the recorder's call.
 *   Excluded inputs: a null object; a sequence pointer to anything but the
 *     block the setup builds.
 *   Not reached by any input: none known (see the coverage line).
 */
#include "../../ps1/src/game.h"
#include "../../ps1/src/protos.h"
#include "../../ps1/src/externs.h"

void func_80146794(Object *object) {
    Object *arg;

    if (object->field_03 >= 0x10) {
        if (object->field_03 == 0x10) {
            object->pos_x = object->pos_x + 4;
        } else if (object->field_03 == 0x11) {
            object->pos_x = object->pos_x - 4;
        }
    }
    if ((s16)object->field_3a < 0) {
        object->field_04 = object->field_04 + 1;
    }
    arg = object;
    if ((u16)(object->field_38 - 1) == 0 && (s16)object->field_3a < 0) {
        arg = (Object *)object->sequence;
    }
    func_80131094(object);
    ((void (*)(Object *))func_80120028)(arg);
}
