/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void finish_sequence_set_other_code_5_slot04_00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}

void func_801b1600_slot04_00(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3 = 1;
        obj->vel_x = 0x80000;
        obj->accel_x = -0x8000;
        obj->vel_y = 0x90000;
        obj->accel_y = -0x6000;
        play_side_sound_at(obj, obj->side, 8);
        set_sequence_by_side_and_mode(obj, 0x2e);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            spawn_offset_effect_and_play_sounds(obj);
        }
        update_sequence(obj);
    }
}
