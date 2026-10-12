/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void clear_three_bytes_if_state_2_zero(Object *object) {
    if (object->state_2 == 0) clear_three_script_bytes(object);
}

void func_8014d8d4(Object *object) {
    if (object->field_209 == 0) {
        clear_three_bytes_if_state_2_zero(object);
    } else if (object->field_209 == 1) {
        clear_script_bytes_if_idle_or_flag_5_off(object);
    }
}

void clear_script_bytes_if_idle_or_flag_5_off(Object *object) {
    if (object->state_2 == 0 || object->active_flag_5 == 0) clear_three_script_bytes(object);
}

void func_8014d95c(Object *object) {
    if (*(u16 *)&object->state_0 == 1 && object->state_2 == 0) clear_three_script_bytes(object);
}

void clear_three_script_bytes(Object *object) {
    object->field_207 = 0;
    object->field_208 = 0;
    object->field_209 = 0;
}

void init_script_from_table_index(Object *object) {
    u16 **table;
    u16 *p;
    u16 first;
    object->script_entry_index = 0;
    object->script_counter = 0;
    if (object->side == 0) table = scr_c4_left;
    else table = scr_174_right;
    p = table[object->script_table_index >> 1];
    data_80189460 = p + 1;
    first = *p;
    object->script_pointer = (s32)data_80189460;
    object->data_ptr = (s32)data_80189460;
    object->field_224 = 0;
    object->field_25d = 0;
    object->field_20c = first;
}
