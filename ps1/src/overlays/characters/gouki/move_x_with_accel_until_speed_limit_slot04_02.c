/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void func_801b359c_slot04_02(Object *obj);

void move_x_with_accel_until_speed_limit_slot04_02(Object *obj) {
    int t;

    if (obj->field_7e == 0) {
        func_801b359c_slot04_02(obj);
    }
    if (obj->variant_index != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    obj->facing_side = obj->saved_facing;
    if (((obj->bound_push_flag >> obj->variant_index) & 1) == 0) {
        t = 0x30000;
        if (obj->input_dir_second != 0) {
            t = 0x24000;
        }
        if (t < obj->vel_x) {
            update_sequence(obj);
            return;
        }
    }
    obj->state_3 = obj->state_3 + 1;
    obj->active_flag_4 = 0;
    obj->countdown = *(u8 *)&obj->countdown | 0xa00;
    set_sequence_by_side(obj, 0);
}

void update_countdown_then_reset_to_idle_slot04_02(Object *obj) {
    obj->countdown = (s16)obj->countdown - 0x100;
    if ((obj->countdown & 0xff00) == 0) {
        obj->facing_side = obj->saved_facing;
        obj->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(obj);
        if (obj->field_7e == 0) {
            call_side_func_then_queue_job(obj);
        }
    } else {
        if (obj->field_7e == 0) {
            func_801b359c_slot04_02(obj);
        }
        start_action_if_buttons_pressed(obj);
        update_sequence(obj);
    }
}
