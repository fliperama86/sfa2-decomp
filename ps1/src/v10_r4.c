/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"
#include "externs.h"

/* What it does (inferred, not original names): per-frame update of an object's button words. When game_state.field_30 is not 0 it checks the object's side against game_state.mode: on a mismatch it clears the object's field_c2, field_130 and field_150, and sets all three to 0x4000 when the flag byte data_801a6984[3] is not 0; on a match it calls func_80131ab4(object) when data_801a6984[1] is not 0. Then, when field_0b is not 0, it swaps bits 0x2000 and 0x8000 of field_130 (bit 13 moves to bit 15 and bit 15 to bit 13); when field_158 is not 0 it does the same for field_150. Last it stores in field_134 the bits of field_130 not in field_132, and in field_136 the bits of field_132 not in field_130 (the pressed and the released bits). */
void func_8013172c(Object *object) {
    unsigned a, b, c, d;
    u16 x;
    s16 y;
    /* The flag bytes are read as elements of data_801a6984 (the headers name [1] and [3] separately as data_801a6985 and data_801a6987 as well; this is the same address). */
    u8 *g = &game_state.field_30;
    if (*g != 0) {
        /* Exception, not recovered source: the read of game_state.mode is written as the pointer g moved back by the distance between field_30 and mode. The value is the same as game_state.mode; the original reads mode at -0x15 from the address of field_30 that it holds in a register, and the plain spellings (game_state.mode, g[-0x15]) build it from a second address. Without it this function differs from the original in 22 instruction lines. */
        if (*(g - (&game_state.field_30 - &game_state.mode)) != object->side + 1) {
            object->field_c2 = 0;
            object->field_130 = 0;
            object->field_150 = 0;
            if (data_801a6984[3] != 0) {
                object->field_c2 = 0x4000;
                object->field_130 = 0x4000;
                object->field_150 = 0x4000;
            }
        } else if (data_801a6984[1] != 0) {
            func_80131ab4(object);
        }
    }
    x = object->field_130;
    y = object->field_150;
    a = (x >> 2) & 0x2000;
    b = (x & 0x2000) << 2;
    c = (y & 0x8000) >> 2;
    d = (y & 0x2000) << 2;
    if (object->field_0b != 0) {
        object->field_130 = (x & 0x5fff) + (a + b);
    }
    if (object->field_158 != 0) {
        object->field_150 = (object->field_150 & 0x5fff) + (c + d);
    }
    /* Exception, not recovered source: the local a is used again for the two last results, as it was for the mask above. It changes no value; the original reuses the register of a here. Written as plain expressions (field_134 = ~field_132 & field_130, and alike) this function differs from the original in 22 instruction lines; with a new local in place of a, in 17. */
    a = object->field_132;
    a = ~a;
    a &= object->field_130;
    object->field_134 = a;
    a = object->field_130;
    a = ~a;
    a &= object->field_132;
    object->field_136 = a;
}
