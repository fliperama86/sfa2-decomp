/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_airborne_until_landing_slot04_02(Object *obj) {
    int n;

    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        play_object_sound_by_kind(obj);
        n = 0x39;
        if (obj->variant_index == 1) {
            n = 0x38;
        }
        set_sequence_by_side_and_mode(obj, n);
    } else {
        update_sequence(obj);
    }
}
