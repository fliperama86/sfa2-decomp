/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_direction_sequence_gain_meter_four_slot04_0b(Object *obj) {
    int a = 0x1a;

    obj->active_flag_4 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 4);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    if (obj->field_49 != 0) {
        a = 0x39;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}

void play_sounds_on_step_flag_1_slot04_0b(Object *obj) {
    if (*(u8 *)&obj->step_flags == 1) {
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xf);
        play_side_sound_at(obj, obj->side, 4);
    }
    update_sequence(obj);
}
