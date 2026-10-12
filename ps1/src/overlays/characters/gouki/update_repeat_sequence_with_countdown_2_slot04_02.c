/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

u8 move_with_gravity_return_above_ground_slot04_02(Object *object);
void land_on_ground_play_sound_set_landing_sequence_slot04_02(Object *object);

void update_repeat_sequence_with_countdown_2_slot04_02(Object *obj) {

    if ((s16)obj->step_flags & 0x8000) {
        play_sound_at(obj, obj->side, 0x320);
        ((Slot04bObj *)obj)->countdown_2--;
        if (((Slot04bObj *)obj)->countdown_2 == 0) {
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x1f);
            return;
        }
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    update_sequence(obj);
}

void update_airborne_or_land_slot04_02(Object *obj) {
    if (move_with_gravity_return_above_ground_slot04_02(obj) == 0) {
        land_on_ground_play_sound_set_landing_sequence_slot04_02(obj);
    } else {
        update_sequence(obj);
    }
}

void finish_sequence_or_set_other_state_5_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        update_sequence(obj);
    }
}
