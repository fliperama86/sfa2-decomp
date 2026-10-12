/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_airborne_or_land_with_variant_sequence_slot04_02(Object *obj) {
    int n;

    if ((u8)apply_velocity_and_check_ground(obj) != 0) {
        if ((s16)obj->step_flags & 0x8000) {
            obj->state_3++;
            obj->active_flag_3 = 0;
            set_sequence_by_side(obj, 0x21);
        } else {
            update_sequence(obj);
        }
    } else {
        obj->state_3 = 4;
        obj->active_flag_3 = 0;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        n = 0x5e;
        if (obj->variant_index != 1) {
            n = 0x5f;
        }
        set_sequence_by_side_and_mode(obj, n);
    }
}

void update_airborne_or_land_set_variant_sequence_slot04_02(Object *obj) {
    int n;

    if ((u8)apply_velocity_and_check_ground(obj) == 0) {
        obj->state_3 = 4;
        obj->active_flag_3 = 0;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        n = 0x5e;
        if (obj->variant_index != 1) {
            n = 0x5f;
        }
        set_sequence_by_side_and_mode(obj, n);
    } else {
        update_sequence(obj);
    }
}
