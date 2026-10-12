/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_3e_or_3f_by_input_bit_slot04_01(Object *obj) {
    int a = 0x3e;

    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    if (obj->input_bits_2 & 0x8000) {
        a = 0x3f;
    }
    set_sequence_by_side_and_mode(obj, a);
}

void update_sequence_play_sound_or_finish_slot04_01(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            play_side_sound_at(obj, obj->side, 9);
        }
        update_sequence(obj);
    }
}
