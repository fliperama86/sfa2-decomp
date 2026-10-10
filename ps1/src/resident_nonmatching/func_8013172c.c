/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice and
 * instruction order. The build does not use this file. The differential
 * test next to it (difftest.py, with func_8013172c.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not original names): per-frame update of an
 * object's button words. When game_state.field_30 is not 0 it checks the
 * object's side against game_state.mode: on a mismatch it clears the
 * object's field_c2, field_130 and field_150, and sets all three to 0x4000
 * when data_801a6987 is not 0; on a match it calls func_80131ab4(object)
 * when data_801a6985 is not 0. Then, when the object's field_0b is not 0,
 * it swaps bits 0x2000 and 0x8000 of field_130 (bit 13 moves to bit 15 and
 * bit 15 to bit 13, the bits of 0x5fff stay); when field_158 is not 0 it
 * does the same for field_150 (bit 15 is taken from the sign of the
 * halfword). Last it stores in field_134 the bits of field_130 that are
 * not in field_132, and in field_136 the bits of field_132 not in
 * field_130 (the pressed and the released bits).
 *
 * Contract:
 *   Argument: a0 = pointer to an object. No return value.
 *   Reads: game_state.field_30 and game_state.mode, data_801a6985 and
 *     data_801a6987 (signed bytes), the object's side, field_0b, field_130,
 *     field_132, field_150, field_158.
 *   Writes: the object's field_c2, field_130, field_134, field_136,
 *     field_150, and whatever func_80131ab4 writes (see below).
 *   Watched at the call (copied by the recorder): the whole object.
 *   Callee: func_80131ab4 takes one argument (the object) and is replaced
 *     by a recorder in both runs; its own effects are outside this test
 *     (it has its own contract).
 *   Aliasing: the object is one block; nothing else is written.
 *   Excluded: none.
 *   Slots no input can reach: none known.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8013172c(Object *object) {
    u16 x;
    s16 y;

    if (game_state.field_30 != 0) {
        if (game_state.mode != object->side + 1) {
            object->field_c2 = 0;
            object->field_130 = 0;
            object->field_150 = 0;
            if (data_801a6987 != 0) {
                object->field_c2 = 0x4000;
                object->field_130 = 0x4000;
                object->field_150 = 0x4000;
            }
        } else if (data_801a6985 != 0) {
            func_80131ab4(object);
        }
    }
    x = object->field_130;
    y = object->field_150;
    if (object->field_0b != 0) {
        object->field_130 = ((x >> 2) & 0x2000) + ((x & 0x2000) << 2) + (x & 0x5fff);
    }
    if (object->field_158 != 0) {
        object->field_150 = (object->field_150 & 0x5fff) + ((y & 0x8000) >> 2) + ((y & 0x2000) << 2);
    }
    object->field_134 = ~object->field_132 & object->field_130;
    object->field_136 = ~object->field_130 & object->field_132;
}
