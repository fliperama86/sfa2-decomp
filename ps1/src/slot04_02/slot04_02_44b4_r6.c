/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void nudge_other_x_by_table_every_fourth_tick_slot04_02(Object *obj);

void slide_with_decaying_vel_x_variant_slot04_02(Object *obj) {
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->state_3 = obj->state_3 + 1;
        set_sequence_by_side_and_mode(obj, 0x56);
        start_action_if_buttons_pressed(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
        nudge_other_x_by_table_every_fourth_tick_slot04_02(obj);
    }
}

void update_sequence_or_clear_and_reset_idle_slot04_02(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->field_157 = 0;
        obj->active_state = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    } else {
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}
