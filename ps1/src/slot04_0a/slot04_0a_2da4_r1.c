/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801c0424_slot04_0a[][21];

void start_jump_toward_other_b_slot04_0a(Object *obj) {
    Object *o;
    int t;

    obj->state_3++;
    obj->active_flag = 0;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    o = obj->other;
    t = *(s32 *)&o->pos_x_frac - *(s32 *)&obj->pos_x_frac;
    obj->vel_y = 0x60000;
    obj->accel_x = 0;
    obj->accel_y = -0x5000;
    obj->vel_x = t >> 5;
    if (o->active_state != 0 || o->field_157 != 0) {
        t = 0;
    } else {
        t = 1;
    }

    t = data_801c0424_slot04_0a[t][o->kind];
    t <<= 16;
    t += *(s32 *)&obj->pos_y_frac - *(s32 *)&o->pos_y_frac;
    if (t > 0x900000) {
        t = 0x900000;
    }
    t >>= 5;
    obj->vel_y = obj->vel_y + t;
    if (obj->field_49 != 0) {
        set_sequence_by_side_and_mode(obj, 0x55);
    } else {
        set_sequence_by_side_and_mode(obj, 0x54);
    }
}
