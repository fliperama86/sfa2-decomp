/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801eca14_slot06_09[];
extern SequenceStep *data_801ec940_slot06_09[];
extern s16 data_801ec954_slot06_09[];
extern s16 data_801ec956_slot06_09[];
extern u16 data_801ec958_slot06_09[];

void set_accel_and_countdown_from_tables_slot06_09(Object *obj);

void release_object_record_slot06_09(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void set_accel_and_countdown_from_tables_slot06_09(Object *obj) {
    int i = (u8)(obj->table_index * 32 + ((Slot06Obj *)obj)->table_base_index);
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];

    obj->accel_x = data_801ec954_slot06_09[i];
    obj->accel_y = data_801ec956_slot06_09[i];
    obj->countdown = data_801ec958_slot06_09[i];
}

void call_handler_by_state_0_2_slot06_09(Object *obj) {
    data_801eca14_slot06_09[obj->state_0](obj);
}

void init_object_vel_x_accel_x_from_position_slot06_09(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->spawn_mode = 4;
    obj->vel_x = obj->pos_x;
    obj->image_row = 0;
    obj->field_0c = 0;
    obj->state_0++;
    obj->accel_x = 0xf0 - obj->pos_y;
    set_sequence_entry(obj, data_801ec940_slot06_09[0]);
}

void update_object_pos_y_from_layer_and_advance_slot06_09(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;

    if (game_state.field_65 == 0) {
        o->pos_y = 0xf0 - (obj->y_offset + ((Slot06Layer *)data_801aa544)->pos_y_int);
        advance_sequence_checked(o);
    }
    if (((Slot06Layer *)data_801aa544)->pos_x_int >= 0x3a0) {
        o->state_0++;
    }
    set_flag_and_push_if_mode_8(o);
}

void release_object_record_2_slot06_09(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
