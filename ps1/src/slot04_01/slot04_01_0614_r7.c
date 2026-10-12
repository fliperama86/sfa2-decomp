/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int move_with_gravity_and_check_ground_slot04_01(Object *obj);
void land_on_ground_set_sequence_slot04_01(Object *obj);

void update_airborne_until_sequence_end_slot04_01(Object *obj) {
    if (move_with_gravity_and_check_ground_slot04_01(obj) != 0) {
        land_on_ground_set_sequence_slot04_01(obj);
    } else if ((s16)obj->step_flags & 0x8000) {
        obj->state_3++;
        set_sequence_by_side(obj, 0x21);
    } else {
        update_sequence(obj);
    }
}

void update_airborne_or_land_slot04_01(Object *obj) {
    if (move_with_gravity_and_check_ground_slot04_01(obj) == 0) {
        update_sequence(obj);
    } else {
        land_on_ground_set_sequence_slot04_01(obj);
    }
}

void land_on_ground_set_sequence_slot04_01(Object *obj) {
    if (obj->active_state != 0) {
        play_object_sound_by_kind(obj);
    }
    obj->state_3++;
    obj->pos_y_frac = 0;
    obj->active_state = 0;
    obj->active_flag_3 = 0;
    obj->pos_y = obj->ground_y;
    set_sequence_by_side(obj, 0x11);
}
