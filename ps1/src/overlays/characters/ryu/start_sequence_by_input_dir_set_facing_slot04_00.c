/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void start_sequence_by_input_dir_set_facing_slot04_00(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->facing_side = 0;
    if (obj->mode_flag_2 == 0) {
        if (obj->input_bits & 0x8000) {
            obj->facing_side = 1;
        }
    }
    set_sequence_by_side_and_mode(obj, (obj->input_dir_second >> 1) + 0x18);
}

void update_sequence_hit_other_on_step_flag_byte_slot04_00(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 5, 0x11, 0, 1, 0);
    }
    update_sequence(obj);
}
