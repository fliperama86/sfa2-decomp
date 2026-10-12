/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 move_ballistic_and_check_ground_slot04_0b(Object *obj);

void update_rise_then_start_fall_sequence_slot04_0b(Object *o) {
    move_ballistic_and_check_ground_slot04_0b(o);
    if (o->vel_y < 0) {
        o->state_3++;
        o->accel_x = 0;
        o->vel_y = 0;
        o->accel_y = 0xffff0000;
        if (o->facing_side == 0) {
            o->vel_x = 0x10000;
        } else {
            o->vel_x = 0xffff0000;
        }
        set_sequence_by_side_and_mode(o, 0x23);
    } else {
        update_sequence(o);
    }
}

void update_airborne_until_ground_landing_2_slot04_0b(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    move_ballistic_and_check_ground_slot04_0b(o);
    if (o->pos_y < o->ground_y) {
        if (obj->mode != 0) {
            return;
        }
    } else {
        o->state_3++;
        o->pos_y_frac = 0;
        o->active_state = 0;
        o->active_flag_4 = 0;
        o->pos_y = (u16)o->ground_y;
        play_object_sound_by_kind(o);
    }
    update_sequence(o);
}
