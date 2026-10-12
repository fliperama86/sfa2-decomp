/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_3_play_sound_on_direction_4_slot04_04(Object *obj);
void update_sequence_or_finish_on_flag_2_slot04_04(Object *obj);
void set_active_flag_3_and_call_direction_sequence_slot04_04(Object *obj);

void call_handler_by_state_3_slot04_04(Object *obj) {
    if (obj->state_3 == 0) {
        advance_state_3_play_sound_on_direction_4_slot04_04(obj);
    } else {
        update_sequence_or_finish_on_flag_2_slot04_04(obj);
    }
}

void advance_state_3_play_sound_on_direction_4_slot04_04(Object *obj) {
    obj->state_3++;
    obj->facing_side = obj->saved_facing;
    if (obj->direction_index == 4) {
        play_side_sound_at(obj, obj->side, 0xb);
    }
    set_active_flag_3_and_call_direction_sequence_slot04_04(obj);
}
