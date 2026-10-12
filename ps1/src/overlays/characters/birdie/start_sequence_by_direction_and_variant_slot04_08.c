#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_by_direction_and_variant_slot04_08(Object *obj) {
    int s = 0xc;

    obj->state_3 = 3;
    obj->active_flag_3 = 1;
    add_to_meter_clamped(obj, obj->direction_index >> 1);
    if (obj->variant_index != 0) {
        s = 0x12;
    }
    if (obj->input_dir_second != 0) {
        s += 3;
    }
    if (obj->attack_param_d != 0) {
        play_side_sound_at(obj, obj->side, 0xc);
        add_to_meter_clamped(obj, 2);
        set_sequence_by_side_and_mode(obj, 0x34);
    } else {
        set_sequence_by_side_and_mode(obj, (s16)((obj->direction_index >> 1) + s));
    }
}
