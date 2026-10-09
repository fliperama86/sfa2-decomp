/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code is 16 bytes longer (the original shares one call of
 * func_801307e0 between the landing arm and the two distance arms; this C
 * calls it in each arm).
 * The exact owner of the bytes in the PS1 build stays the raw bytes of the
 * module image; the build does not use this file. The differential test next
 * to it (difftest.py, with the contract func_801b339c_slot04_0e.py) compares
 * the behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): one step of a jump or fall
 * toward a target height, field_70. func_80130184 moves the object by its
 * velocity words and reports whether pos_y has reached field_70. When it
 * has, the object lands: field_10, field_45 and field_159 are cleared, the
 * state byte field_07 advances, pos_y is set to the target, a sound is
 * played and sequence 0x36 starts. Otherwise the remaining distance
 * (target minus pos_y, as a signed halfword) picks a sequence: 0x3b below
 * 0x11, 0x3a below 0x21, and for a larger distance the current sequence
 * simply steps on (func_80130efc).
 *
 * Contract (what the code reads and writes; the roles named for the
 * fields are inferred):
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: the object's side (field_a6), field_07, pos_y, field_70, and
 *     what the callees read.
 *   Writes: in the landing arm field_10, field_45, field_159, field_07,
 *     pos_y; otherwise only what the callees write.
 *   Callees run as original code: func_80130184 (reads and writes the
 *     velocity words and fractions at field_10 to field_58, reads pos_y and
 *     field_70), func_801307e0 and func_80130efc (the sequence tables in the
 *     scratchpad, the sequence steps and frame records; they write the
 *     object's sequence, field_38, field_3a, field_80, frame, field_4a and
 *     may step the sequence further).
 *   Callee replaced by a recorder (no result, log in RAM): func_801204f4
 *     (3 arguments), which ends in sound routines of Sony's library.
 *   Watched by the recorders: the whole object (0x394 bytes), copied at every
 *     recorded call. No recorded callee takes a pointer to memory the
 *     function fills.
 *   Aliasing: the object, the sequence steps, the frame records and the
 *     pointer tables are distinct blocks.
 *   Not reached by any input: none known; see the test lines for the
 *     coverage.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b339c_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s16 d;

    if (func_80130184(o) == 0) {
        o->field_10 = 0;
        o->field_45 = 0;
        o->field_159 = 0;
        o->field_07++;
        o->pos_y = obj->field_70;
        func_801204f4(o, o->side, 0x1b);
        func_801307e0(o, 0x36);
    } else {
        d = obj->field_70 - (u16)o->pos_y;
        if (d < 0x11) {
            func_801307e0(o, 0x3b);
        } else if (d < 0x21) {
            func_801307e0(o, 0x3a);
        } else {
            func_80130efc(o);
        }
    }
}
