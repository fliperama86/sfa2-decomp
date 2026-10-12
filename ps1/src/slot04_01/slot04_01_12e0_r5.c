/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801befa4_slot04_01[];

void start_rise_with_facing_table_vel_x_and_sound_slot04_01(Object *obj) {
    s32 v;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    play_side_sound_at(obj, obj->side, 0xc);
    obj->vel_y = 0x40000;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac & 0xffff0000;
    obj->active_state = 1;
    if (obj->field_49 == 0) {
        obj->accel_y = -0x6000;
    } else {
        obj->accel_y = -0x18000;
    }
    v = data_801befa4_slot04_01[obj->direction_index >> 1];
    if (obj->facing_side == 0) {
        obj->vel_x = -v;
    } else {
        obj->vel_x = v;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x32);
}

void update_rise_then_start_fall_sequence_b_slot04_01(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    int a = 0x22;

    *(s32 *)&o->pos_x_frac = *(s32 *)&o->pos_x_frac + o->vel_x;
    *(s32 *)&o->pos_y_frac = *(s32 *)&o->pos_y_frac - o->vel_y;
    o->vel_y = o->vel_y + o->accel_y;
    if (o->vel_y < 0) {
        o->state_3++;
        play_sound_at(o, o->side, 0x320);
        obj->step_count = (o->direction_index >> 1) + 1;
        if (o->field_49 != 0) {
            a = 0x4a;
        }
        set_sequence_by_side_and_mode(o, obj->step_count + a - 1);
    } else {
        update_sequence(o);
    }
}
