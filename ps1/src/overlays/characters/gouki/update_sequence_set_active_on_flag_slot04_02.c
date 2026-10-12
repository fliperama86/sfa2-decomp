/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void apply_vel_y_with_accel_y_override_slot04_02(Object *obj);

void update_sequence_set_active_on_flag_slot04_02(Object *o) {
    if (*(u8 *)&o->step_flags == 0) {
        o->active_state = 1;
        o->state_3++;
    }
    update_sequence(o);
}

void update_airborne_motion_land_with_sound_slot04_02(Object *obj) {
    apply_vel_y_with_accel_y_override_slot04_02(obj);
    if (obj->pos_y < obj->ground_y) {
        if (obj->facing_side != 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
        if (obj->vel_x < 0) {
            obj->state_3++;
        }
    } else {
        obj->active_flag_4 = 0;
        obj->field_49 = 0;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
    }
    update_sequence(obj);
}

void update_fall_until_landing_or_mode_set_slot04_02(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s16 y;

    apply_vel_y_with_accel_y_override_slot04_02(o);
    y = o->ground_y;
    if (o->pos_y < y) {
        if (obj->mode != 0) {
            return;
        }
    } else {
        o->state_3++;
        o->pos_y = y;
        o->active_flag_4 = 0;
        o->active_flag_3 = 0;
        o->active_state = 0;
        play_object_sound_by_kind(o);
    }
    update_sequence(o);
}
