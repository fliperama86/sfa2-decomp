/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_x_by_facing_with_decel_stop_slot04_07(Object *obj);

void update_slide_set_other_code_until_end_slot04_07(Object *obj) {
    update_sequence(obj);
    move_x_by_facing_with_decel_stop_slot04_07(obj);
    obj->other->other_state_code = 5;
    if ((s16)obj->step_flags & 0x8000) {
        reset_object_to_state_1_0_sequence_0(obj);
    }
}

void move_x_by_facing_with_decel_stop_slot04_07(Object *obj) {
    if (obj->facing_side != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
}
