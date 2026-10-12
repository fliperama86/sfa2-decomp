/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_facing_from_input_or_position_and_start_slot04_04(Object *obj) {
    obj->state_3++;
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    add_to_meter_clamped(obj, 3);
    obj->facing_side = 0;
    if (obj->mode_flag_2 != 0) {
        if (!((s16)(box_margin[0] + 0xc0) < obj->pos_x)) {
            obj->facing_side = 1;
        }
    } else if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
    }
    set_sequence_by_side_and_mode(obj, 0x18);
}

void hit_other_with_sound_on_flag_byte_then_update_slot04_04(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        play_side_sound_at(obj, obj->side, 0xc);
        set_other_hit_reaction_state_0(obj, 4, 0xa, 0xf, 0, 0, 1);
    }
    update_sequence(obj);
}
