/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void land_on_ground_start_sequence_forty_eight_slot04_0e(Object *obj);
void set_direction_bytes_from_byte_pair_slot04_0e(Object *obj);

void land_on_ground_start_sequence_forty_eight_slot04_0e(Object *obj) {
    obj->state_3 = 6;
    obj->active_state = 0;
    obj->pos_y_frac = 0;
    obj->active_flag_4 = 0;
    obj->pos_y = (u16)obj->ground_y;
    play_object_sound_by_kind(obj);
    set_sequence_by_side_and_mode(obj, 0x30);
}

void update_sequence_then_launch_jump_on_end_slot04_0e(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (obj->state_flag == 0) {
        set_direction_bytes_from_byte_pair_slot04_0e(o);
    }
    if ((s16)o->step_flags >= 0) {
        update_sequence(o);
    } else {
        o->vel_x = 0x60000;
        o->vel_y = 0x90000;
        o->accel_y = -0x8000;
        o->state_3++;
        o->accel_x = 0;
        if (o->facing_side == 0) {
            o->vel_x = -o->vel_x;
        }
        set_sequence_by_side_and_mode(o, 0x2f);
    }
}
