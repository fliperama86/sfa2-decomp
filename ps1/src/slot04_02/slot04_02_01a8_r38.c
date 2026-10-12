/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void nudge_other_x_by_table_every_fourth_tick_slot04_02(Object *obj);

void set_state_3_9_direction_sequence_slot04_02(Object *obj) {
    obj->state_3 = 9;
    obj->active_flag_3 = 1;
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x52);
}

void slide_with_decaying_vel_x_slot04_02(Object *obj) {
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->state_3 = obj->state_3 + 1;
        obj->active_flag_4 = 0;
        obj->step_timer = 1;
        set_sequence_by_side_and_mode(obj, 0x50);
    } else {
        if ((u8)obj->step_flags == 0) {
            update_sequence(obj);
        }
        nudge_other_x_by_table_every_fourth_tick_slot04_02(obj);
    }
}
