/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_sequence_facing_from_input_with_meter_gain_slot04_03(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    obj->facing_side = 0;
    if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
    }
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    add_to_meter_clamped(obj, 3);
    set_sequence_by_side_and_mode(obj, 0x18);
}

void apply_damage_and_sound_on_step_flag_then_update_slot04_03(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        game_state.state_param = 0x30;
        if ((apply_damage_to_other_return_defeated(obj, 0x10, 0) & 0xff) != 0) {
            play_sound_at(obj, obj->side, 0x34b);
        } else {
            play_sound_at(obj, obj->side, 0x319);
        }
        spawn_two_effects_at_other_ground(obj);
    }
    update_sequence(obj);
}

void hit_other_on_step_flag_bit_then_update_slot04_03(Object *obj) {
    if (obj->step_flags & 0x80) {
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 0xf, -0x200, 0, 0, 1);
    }
    update_sequence(obj);
}
