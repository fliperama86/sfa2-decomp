/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80027c6c_slot27[];
extern ObjectFn data_80027c7c_slot27[];
extern ObjectFn data_80027cac_slot27[];
void init_object_from_table_index_entry_slot27(Object *obj);
void set_sequence_from_table_index_slot27(Object *obj);

void call_handler_by_state_0_3_slot27(Object *obj) {
    data_80027c6c_slot27[obj->state_0](obj);
}

void advance_state_0_and_init_table_entry_sequence_slot27(Object *obj) {
    obj->state_0 = obj->state_0 + 1;
    init_object_from_table_index_entry_slot27(obj);
    set_sequence_from_table_index_slot27(obj);
}

void call_table_handler_then_advance_sequence_slot27(Object *obj) {
    data_80027c7c_slot27[obj->table_index](obj);
    advance_sequence_checked(obj);
}

void func_80014480_slot27(Object *obj) {
    data_80027cac_slot27[obj->state_1](obj);
}

void start_horizontal_move_by_table_index_slot27(Object *obj) {
    obj->spawn_x = 0x20;
    obj->spawn_y = 0x1e0;
    obj->field_01 = 1;
    obj->state_1 = obj->state_1 + 1;
    if (obj->table_index == 10) {
        obj->vel_x = 8;
    } else {
        obj->vel_x = -8;
    }
}
