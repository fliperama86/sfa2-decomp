/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014dbc8(Object *object) {
    u16 **table;
    u16 *p;
    u8 index;
    u16 first;
    object->field_253 = 0;
    if (object->side == 0) table = scr_d0_left;
    else table = scr_180_right;
    index = object->field_244;
    object->field_24f = index;
    p = table[index];
    data_80189460 = p + 1;
    first = *p;
    object->field_234 = (s32)data_80189460;
    object->field_224 = 0;
    object->field_25d = 0;
    object->field_20c = first;
}

void func_8014dc30(Object *object) {
    u8 v = data_8017d320[object->field_cf];
    object->field_21c = v;
    if (object->kind == 0x14) object->field_21c = v - 10;
    object->field_21c = data_8017d300[func_80151184() & 0x1f] + object->field_21c - 4;
    if (object->field_21c < 0) object->field_21c = 0;
}

u8 func_8014dcc0(Object *object) {
    if (object->side == 0) scr_d4_left();
    else scr_184_right();
    return data_801ad398;
}
