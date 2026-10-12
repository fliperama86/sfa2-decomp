/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_air_move_sequence_by_direction_slot04_08(Object *obj) {
    int k;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    set_direction_from_pressed_bits(obj);
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    k = 0xc;
    if (obj->variant_index != 0) {
        k = 0x12;
    }
    if (obj->input_dir_second != 0) {
        k += 3;
    } else if (obj->direction_index == 4 && (obj->input_bits_2 & 0x4000) != 0) {
        play_side_sound_at(obj, obj->side, 0xc);
        add_to_meter_clamped(obj, 2);
        set_sequence_by_side_and_mode(obj, 0x34);
        return;
    }
    set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + k));
}
