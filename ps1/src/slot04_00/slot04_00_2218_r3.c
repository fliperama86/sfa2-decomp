/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int move_with_gravity_and_check_ground_slot04_00(Object *obj);

void update_descend_slowly_by_step_count_slot04_00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        if (--((Slot04aObj *)obj)->step_count == 0) {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x35);
            return;
        }
    }
    *(s32 *)&obj->pos_y_frac += 0x2000;
    update_sequence(obj);
}

void update_airborne_land_with_sound_slot04_00(Object *obj) {
    if (move_with_gravity_and_check_ground_slot04_00(obj) != 0) {
        obj->state_3++;
        play_object_sound_by_kind(obj);
        set_sequence_by_side_and_mode(obj, 0x30);
    } else {
        update_sequence(obj);
    }
}
