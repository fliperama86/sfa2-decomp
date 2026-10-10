/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice (the
 * original keeps a sign-extended copy of its selector that this C does not
 * need). The exact owner of the bytes in the PS1 build stays the raw bytes
 * of the module image; the build does not use this file. The differential
 * test next to it (difftest.py, with func_801b2e8c_slot04_0e.py) compares
 * the behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): a character action handler
 * that first integrates the object's motion (func_801b4914_slot04_0e), then
 * looks at the object's height field_70 against its position pos_y. Above
 * the limit it switches to the action of func_801b2f58_slot04_0e; within
 * 0x30 below it only advances the animation (func_80130efc); otherwise, if
 * field_164 equals the selector for the facing (2 when field_0b is 0, else
 * 1), it counts up field_07, clears field_50 and field_58, starts sequence
 * 0x2e (0x60 when field_49 is not 0) and calls func_801209c4; if field_164
 * differs it only advances the animation.
 *
 * Contract:
 *   Argument: a0 = pointer to an object (Object, 0x394 bytes). No return
 *   value.
 *   Reads: the object's words at 0x10, 0x14 (field_14 together with pos_y),
 *     0x4c, 0x50, 0x54 and 0x58 (motion, through func_801b4914_slot04_0e),
 *     field_70, pos_y, field_0b, field_164, field_49, field_07.
 *   Writes: those motion words; in the last arm field_07, field_50,
 *     field_58; in the first arm what func_801b2f58_slot04_0e writes
 *     (field_07, field_45, field_14, field_17b, pos_y).
 *   Callees: func_801b4914_slot04_0e and func_801b2f58_slot04_0e run as
 *     original code (the same in both runs). func_80130efc (1 argument),
 *     func_801307e0 (2 arguments) and func_801209c4 (1 argument) are
 *     replaced by recorders returning 0; they reach sequence tables and the
 *     library (func_801b2f58_slot04_0e calls the last two, so those are
 *     recorded there too).
 *   Watched in the test: the whole object, copied by every recorder at every
 *     call (so the order of the stores against the calls is compared). No
 *     recorded callee gets a pointer to memory filled for the call.
 *   Aliasing: the object is one block; nothing else is written.
 *   Excluded inputs: none. Instruction slots no input can reach: none.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2f58_slot04_0e(Object *obj);
void func_801b4914_slot04_0e(Object *obj);

void func_801b2e8c_slot04_0e(Object *o) {
    int facing;
    int sequence;

    func_801b4914_slot04_0e(o);
    if (o->field_70 < o->pos_y) {
        func_801b2f58_slot04_0e(o);
    } else if ((s16)(o->field_70 - 0x30) < o->pos_y) {
        func_80130efc(o);
    } else {
        if (o->field_0b == 0) facing = 2;
        else facing = 1;
        if (o->field_164 != facing) {
            func_80130efc(o);
        } else {
            sequence = 0x60;
            o->field_07++;
            o->field_50 = 0;
            o->field_58 = 0;
            if (o->field_49 == 0) sequence = 0x2e;
            func_801307e0(o, sequence);
            func_801209c4(o);
        }
    }
}
