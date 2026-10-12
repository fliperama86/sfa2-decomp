/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_delayed_launch_fall_and_land_slot04_02(Object *obj) {
    s16 t = obj->countdown;
    int n;

    if ((t & 0xff00) != 0) {
        t = t - 0x100;
        obj->countdown = t;
        if ((t & 0xff00) == 0) {
            obj->vel_y = 0x74000;
            obj->accel_y = -0x5000;
            if (obj->facing_side != 0) {
                obj->vel_x = -0x10000;
                obj->accel_x = 0x280;
            } else {
                obj->vel_x = 0x10000;
                obj->accel_x = -0x280;
            }
        } else {
            update_sequence(obj);
        }
    } else if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        if ((s16)obj->step_flags < 0) {
            obj->active_flag_3 = 0;
            obj->state_3 = obj->state_3 + 1;
            set_sequence_by_side(obj, 0x21);
        } else {
            update_sequence(obj);
        }
    } else {
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        n = 0x39;
        if (obj->variant_index == 1) {
            n = 0x38;
        }
        set_sequence_by_side_and_mode(obj, n);
    }
}
