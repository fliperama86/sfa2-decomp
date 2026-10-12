/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea9d0_slot06_10[];
extern SequenceStep *data_801ebbb8_slot06_10[];

void set_flag_and_push_if_mode_8(Object *o);

void release_object_record_5_slot06_10(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_6_slot06_10(Object *obj) {
    data_801ea9d0_slot06_10[obj->state_0](obj);
}

void init_object_with_sequence_1_low_slot06_10(Object *obj) {
    obj->field_0a = 1;
    obj->state_0 = 1;
    obj->base_x = 0x340;
    obj->base_y = 0x100;
    obj->spawn_x = 0x70;
    obj->spawn_y = 0x1e0;
    obj->pos_y += 0xe0;
    obj->image_row = 0;
    obj->spawn_mode = 4;
    set_sequence_entry(obj, data_801ebbb8_slot06_10[1]);
}

void load_velocities_from_table_slot06_10(Object *obj) {
    set_flag_and_push_if_mode_8(obj);
}

void release_object_record_6_slot06_10(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
