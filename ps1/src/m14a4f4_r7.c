/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014da18(Object *object) {
    u32 *table;
    u16 *list;
    u16 *p;
    u16 *next;
    u16 value;
    u16 first;
    object->field_253 = 0;
    if (object->side == 0) table = scr_c8_left;
    else table = scr_178_right;
    list = (u16 *)table[object->field_20b];
    table = (u32 *)table[object->field_20b + 1];
    value = list[object->field_220 + (func_80151184() & 0x1f)];
    object->field_24f = value;
    p = (u16 *)table[(s16)value];
    next = p + 1;
    data_80189460 = next;
    first = *p;
    object->field_22c = (s32)next;
    object->field_224 = 0;
    object->field_25d = 0;
    object->field_20c = first;
}

void func_8014dae8(Object *object) {
    u32 *table;
    u16 *list;
    u16 *p;
    u16 *next;
    u16 value;
    u16 first;
    int index;
    object->field_253 = 0;
    if (object->side == 0) table = scr_cc_left;
    else table = scr_17c_right;
    index = object->other->frame->field_0c * 2;
    list = (u16 *)table[index];
    table = (u32 *)table[index + 1];
    value = list[object->field_220 + (func_80151184() & 0x1f)];
    object->field_24f = value;
    p = (u16 *)table[(s16)value];
    next = p + 1;
    data_80189460 = next;
    first = *p;
    object->field_230 = (s32)next;
    object->field_224 = 0;
    object->field_25d = 0;
    object->field_20c = first;
}
