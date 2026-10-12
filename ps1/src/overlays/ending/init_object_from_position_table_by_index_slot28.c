/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern HalfPair data_80051820_slot28[];

void init_object_from_position_table_by_index_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    obj->field_01 = 1;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->state_0 = obj->state_0 + 1;
    obj->pos_x = data_80051820_slot28[obj->table_index].pos_x;
    obj->pos_y = data_80051820_slot28[obj->table_index].pos_y;
    if (obj->table_index == 0) {
        set_sequence_from_table_5(obj, 1, (SequenceStep **)obj->box_tables);
    } else {
        set_sequence_from_table_5(obj, 2, (SequenceStep **)obj->box_tables);
    }
}

void set_flag_and_advance_sequence_checked_slot28(Object *obj) {
    obj->field_01 = 1;
    advance_sequence_checked(obj);
}
