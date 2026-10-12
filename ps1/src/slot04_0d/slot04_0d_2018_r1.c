/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2a50_slot04_0d[];
extern s32 data_801c27d4_slot04_0d[];

void apply_gravity_vertical_motion_slot04_0d(Object *obj) {
    s16 t = ((s16 *)&obj->vel_y)[1];

    if (t > 0) {
        obj->accel_y = 0x5000;
    }
    if (obj->field_49 != 0 && t > 0) {
        obj->accel_y = 0x10000;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac + obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
}

void call_table_function_by_state_3_10_slot04_0d(Object *obj) {
    data_801c2a50_slot04_0d[obj->state_3](obj);
}

void start_sequence_from_direction_index_add_meter_slot04_0d(Object *obj) {
    u16 a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    obj->field_157 = 0;
    a = 0x43;
    if (obj->field_49 == 0) {
        a = 0x31;
    }
    a += obj->direction_index >> 1;
    set_sequence_by_side_and_mode(obj, a);
}

void launch_from_table_on_step_flag_slot04_0d(Object *obj) {
    s32 *t = data_801c27d4_slot04_0d;
    s32 *p;

    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->active_state = 1;
        obj->state_3++;
        p = t + obj->direction_index * 2;
        obj->step_flags = obj->step_flags & 0xff00;
        obj->vel_x = p[0];
        obj->vel_y = p[1];
        obj->accel_x = p[2];
        obj->accel_y = p[3];
        obj->vel_y = -obj->vel_y;
        obj->accel_y = -obj->accel_y;
    }
}
