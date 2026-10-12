/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern void (*data_801dd5b0_slot05_06[])(Object *);
extern Object *data_801dd754_slot05_06;
void follow_parent_position_and_sequence_slot05_06(Object *obj);

void start_direction_sequence_state_3_slot05_06(Object *obj) {
    u16 t;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    t = 0x12;
    if (obj->variant_index == 0) {
        t = 0xc;
    }
    if (obj->input_dir_second != 0) {
        t += 3;
    }
    t = (obj->direction_index >> 1) + t;
    set_sequence_by_side_and_mode(obj, t);
}

void release_object_record_slot05_06(Object *obj) {
    reset_and_push_block_object_type_8((Slab172 *)obj);
}

void call_handler_by_state_0_with_parent_slot05_06(Object *obj) {
    data_801dd754_slot05_06 = obj->parent;
    data_801dd5b0_slot05_06[obj->state_0](obj);
}

void init_object_from_global_object_slot05_06(Object *obj) {
    Object *p;

    obj->state_0++;
    p = data_801dd754_slot05_06;
    obj->field_1c = p->field_1c;
    obj->cycle_value = p->cycle_value;
    obj->table_index = p->kind;
    obj->field_0c = data_801dd754_slot05_06->field_0c;
    obj->image_row = data_801dd754_slot05_06->image_row;
    obj->field_0e = data_801dd754_slot05_06->field_0e;
    obj->variant_index = 0;
    follow_parent_position_and_sequence_slot05_06(obj);
}
