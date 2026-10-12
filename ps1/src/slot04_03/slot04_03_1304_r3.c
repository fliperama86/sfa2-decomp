/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_direction_sequence_with_meter_and_sounds_slot04_03(Object *obj) {
    int a = 0x22;

    obj->active_flag_4 = 1;
    obj->field_225 = 1;
    obj->state_3++;
    add_to_meter_clamped(obj, 5);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    play_side_sound_at(obj, obj->side, 4);
    play_side_sound_at(obj, obj->side, 0xc);
    if (obj->field_49 != 0) {
        a = 0x43;
    }
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
}
