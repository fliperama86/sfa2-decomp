/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void start_jump_away_with_sound_and_position_slot04_05(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->active_state = 1;
    obj->facing_side = 0;
    if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 1;
        obj->vel_x = -0x20000;
    } else {
        obj->vel_x = 0x20000;
    }
    obj->vel_y = 0x60000;
    obj->accel_x = 0;
    obj->accel_y = -0x6000;
    set_sequence_by_side_and_mode(obj, 0x18);
    set_position_from_other_box(obj);
}

void follow_other_box_until_step_flag_hit_slot04_05(Object *obj) {
    update_sequence(obj);
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 5, 0xf, 0, 0, 1);
    } else {
        set_position_from_other_box(obj);
    }
}
