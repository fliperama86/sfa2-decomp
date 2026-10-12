/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int move_with_gravity_and_check_ground_slot04_00(Object *obj);
void land_on_ground_set_sequence_slot04_00(Object *obj);

void update_airborne_loop_with_step_count_slot04_00(Object *obj) {
    if (move_with_gravity_and_check_ground_slot04_00(obj) != 0) {
        land_on_ground_set_sequence_slot04_00(obj);
    } else {
        if ((s16)obj->step_flags & 0x8000) {
            play_sound_at(obj, obj->side, 0x320);
            ((Slot04aObj *)obj)->step_count--;
            if (((Slot04aObj *)obj)->step_count & 0x80) {
                obj->state_3++;
                set_sequence_by_side_and_mode(obj, 0x31);
                return;
            }
        }
        update_sequence(obj);
    }
}

void update_airborne_until_sequence_end_slot04_00(Object *obj) {
    if (move_with_gravity_and_check_ground_slot04_00(obj) != 0) {
        land_on_ground_set_sequence_slot04_00(obj);
    } else if ((s16)obj->step_flags & 0x8000) {
        obj->state_3++;
        set_sequence_by_side(obj, 0x21);
    } else {
        update_sequence(obj);
    }
}

void update_airborne_or_land_slot04_00(Object *obj) {
    if (move_with_gravity_and_check_ground_slot04_00(obj) == 0) {
        update_sequence(obj);
    } else {
        land_on_ground_set_sequence_slot04_00(obj);
    }
}
