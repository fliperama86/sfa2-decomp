/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void branch_on_end_or_repeat_counter_variant_a_slot04_03(Object *obj) {
    s16 t = obj->step_flags;
    u8 c;

    if (t & 0x8000) {
        obj->state_3 = 0xb;
        set_sequence_by_side_and_mode(obj, 0x2f);
    } else if (t == 0) {
        c = obj->input_dir_second - 1;
        obj->input_dir_second = c;
        if (c & 0x80) {
            obj->state_3 = 0xb;
            set_sequence_by_side_and_mode(obj, 0x2f);
        } else {
            obj->state_3++;
            play_side_sound_at(obj, obj->side, 6);
            play_side_sound_at(obj, obj->side, 0xc);
            set_sequence_by_side_and_mode(obj, 0x2c);
        }
    } else {
        update_sequence(obj);
    }
}

void branch_on_end_or_repeat_counter_variant_b_slot04_03(Object *obj) {
    s16 t = obj->step_flags;
    u8 c;

    if (t & 0x8000) {
        obj->state_3 = 0xb;
        set_sequence_by_side_and_mode(obj, 0x30);
    } else if (t == 0) {
        if (obj->direction_index == 0) {
            goto set;
        }
        c = obj->input_dir_second - 1;
        obj->input_dir_second = c;
        if (c & 0x80) {
        set:
            obj->state_3 = 0xb;
            set_sequence_by_side_and_mode(obj, 0x30);
        } else {
            obj->state_3++;
            play_side_sound_at(obj, obj->side, 6);
            play_side_sound_at(obj, obj->side, 0xc);
            set_sequence_by_side_and_mode(obj, 0x2d);
        }
    } else {
        update_sequence(obj);
    }
}
