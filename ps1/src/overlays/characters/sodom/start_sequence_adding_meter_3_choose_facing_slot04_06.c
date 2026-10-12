/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_adding_meter_3_choose_facing_slot04_06(Object *obj) {
    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj->other, obj->other->side, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if ((s16)(box_margin[0] + 0xc0) < obj->pos_x) {
            obj->facing_side = 1;
        }
    } else if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
    }
    set_sequence_by_side_and_mode(obj, 0x18);
}

void hit_other_by_input_dir_when_mode_set_slot04_06(Object *obj) {
    if (((Slot04aObj *)obj)->mode != 0) {
        obj->state_3++;
        if (obj->input_dir_second == 0) {
            set_other_hit_reaction_state_0(obj, 0x26, 5, 0x11, 0, 0, 1);
        } else {
            set_other_hit_reaction_state_0(obj, 0x29, 5, 1, 0, 0, 1);
        }
    }
    update_sequence(obj);
}
