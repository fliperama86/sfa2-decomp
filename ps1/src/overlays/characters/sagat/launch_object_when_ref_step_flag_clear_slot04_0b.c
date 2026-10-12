/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void set_sequence_from_other_side_table_slot04_0b(Object *obj, int index);
void set_position_relative_to_ref_other_slot04_0b(Object *obj, int dx, int dy);

void launch_object_when_ref_step_flag_clear_slot04_0b(Object *obj) {
    if (!(ref_other.p->step_flags & 1)) {
        obj->state_1++;
        if (obj->facing_side != 0) {
            obj->vel_x = 0xfff80000;
        } else {
            obj->vel_x = 0x80000;
        }
        obj->vel_y = 0x20000;
        obj->accel_x = 0;
        obj->accel_y = -0x4000;
        set_position_relative_to_ref_other_slot04_0b(obj, -0x44, 0x38);
        set_sequence_from_other_side_table_slot04_0b(obj, 0x18);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void update_arc_motion_until_ground_reached_slot04_0b(Object *obj) {
    GameState *g = &game_state;
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_x += obj->accel_x;
    obj->vel_y += obj->accel_y;
    if (obj->vel_y < 0) {
        if (!(obj->pos_y < ref_other.p->ground_y)) {
            obj->state_0++;
        }
    }
    set_state_0_if_x_outside_margin_narrow(obj);
    if (g->frame_counter & 1) {
        clear_or_set_flag_by_pos_x_limit(obj);
    }
}
