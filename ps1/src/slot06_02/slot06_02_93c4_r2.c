/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ef0d8_slot06_02[];
extern SequenceStep *data_801ef0c8_slot06_02[];

void call_handler_by_state_0_slot06_02(Object *obj) {
    data_801ef0d8_slot06_02[obj->state_0](obj);
}

void init_object_velocity_from_position_and_sequence_slot06_02(Object *obj) {
    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->field_0c = 0;
    obj->vel_x = *(s32 *)&obj->pos_x_frac;
    obj->vel_y = 0x1000000 - *(s32 *)&obj->pos_y_frac;
    set_sequence_entry(obj, data_801ef0c8_slot06_02[obj->table_index]);
}

void set_object_position_from_camera_bounds_slot06_02(Object *obj) {
    Slot06Layer *l = (Slot06Layer *)cam_obj;
    s32 a;
    s32 b;

    a = *(s32 *)&l->bound_x_c - *(s32 *)&l->bound_x_a;
    b = a;
    a >>= 2;
    a += b;
    *(s32 *)&obj->pos_x_frac = obj->vel_x - a;
    a = *(s32 *)&l->bound_y_b - *(s32 *)&l->bound_y_a;
    b = a >> 2;
    a += b;
    *(s32 *)&obj->pos_y_frac = 0xf80000 - (obj->vel_y - a);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void release_object_record_slot06_02(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
