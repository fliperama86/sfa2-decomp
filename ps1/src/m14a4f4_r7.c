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
    object->script_counter = 0;
    if (object->side == 0) table = scr_c8_left;
    else table = scr_178_right;
    list = (u16 *)table[object->script_table_index];
    table = (u32 *)table[object->script_table_index + 1];
    value = list[object->list_offset + (get_random_byte() & 0x1f)];
    object->script_entry_index = value;
    p = (u16 *)table[(s16)value];
    next = p + 1;
    data_80189460 = next;
    first = *p;
    object->script_ptr_1 = (s32)next;
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
    object->script_counter = 0;
    if (object->side == 0) table = scr_cc_left;
    else table = scr_17c_right;
    index = object->other->frame->field_0c * 2;
    list = (u16 *)table[index];
    table = (u32 *)table[index + 1];
    value = list[object->list_offset + (get_random_byte() & 0x1f)];
    object->script_entry_index = value;
    p = (u16 *)table[(s16)value];
    next = p + 1;
    data_80189460 = next;
    first = *p;
    object->script_ptr_2 = (s32)next;
    object->field_224 = 0;
    object->field_25d = 0;
    object->field_20c = first;
}
