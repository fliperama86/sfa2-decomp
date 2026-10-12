/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5c6c_slot04_09(Object *obj);
void apply_velocity_and_acceleration_slot04_09(Object *obj);

void update_sequence_with_flag_step_move_x_slot04_09(Object *obj) {
    s16 t;
    s16 a;
    obj->countdown--;
    update_sequence(obj);
    func_801b5c6c_slot04_09(obj);
    if (obj->step_flags & 0x80) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
    }
    t = obj->step_flags & 0x7f;
    if (t != 0) {
        if (obj->facing_side != 0) {
            a = t;
        } else {
            a = -t;
        }
        obj->pos_x = a + obj->pos_x;
        obj->step_flags = obj->step_flags & 0xff00;
    }
}

void func_801b4cc0_slot04_09(Object *obj) {
    int a = 0x3c;
    int t = obj->countdown - 1;
    obj->countdown = t;
    if ((t << 16) < 0) {
        obj->state_3++;
        if (obj->field_49 != 0) {
            a = 0x4f;
        }
        set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + a);
    } else {
        apply_velocity_and_acceleration_slot04_09(obj);
        if (obj->vel_x == 0) {
            obj->accel_x = 0;
        }
        if ((u8)obj->step_flags != 0) {
            obj->active_flag_4 = 0;
            start_action_if_buttons_pressed(obj);
        } else {
            func_801b5c6c_slot04_09(obj);
        }
        update_sequence(obj);
    }
}
