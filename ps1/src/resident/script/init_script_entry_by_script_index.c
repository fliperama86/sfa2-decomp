/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void init_script_entry_by_script_index(Object *object) {
    u16 **table;
    u16 *p;
    u8 index;
    u16 first;
    object->script_counter = 0;
    if (object->side == 0) table = scr_d0_left;
    else table = scr_180_right;
    index = object->script_index;
    object->script_entry_index = index;
    p = table[index];
    data_80189460 = p + 1;
    first = *p;
    object->script_ptr_3 = (s32)data_80189460;
    object->field_224 = 0;
    object->field_25d = 0;
    object->field_20c = first;
}

void set_countdown_from_table_with_random(Object *object) {
    u8 v = data_8017d320[object->table_column];
    object->countdown_3 = v;
    if (object->kind == 0x14) object->countdown_3 = v - 10;
    object->countdown_3 = data_8017d300[get_random_byte() & 0x1f] + object->countdown_3 - 4;
    if (object->countdown_3 < 0) object->countdown_3 = 0;
}

u8 func_8014dcc0(Object *object) {
    if (object->side == 0) scr_d4_left();
    else scr_184_right();
    return data_801ad398;
}
