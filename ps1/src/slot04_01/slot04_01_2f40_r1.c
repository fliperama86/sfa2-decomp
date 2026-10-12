/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_horizontal_move_with_fixed_sequence_slot04_01(Object *obj) {
    obj->state_3++;
    obj->pos_y_frac = 0;
    obj->active_flag_4 = 1;
    obj->field_29c = 2;
    obj->active_state = 1;
    if (obj->facing_side == 0) {
        *(s32 *)&obj->vel_x = -0x40000;
    } else {
        *(s32 *)&obj->vel_x = 0x40000;
    }
    set_sequence_by_side_and_mode(obj, 0x48);
}

void update_sequence_step_shift_x_and_move_slot04_01(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        obj->state_3++;
        if (obj->facing_side == 0) {
            obj->pos_x -= 0x14;
        } else {
            obj->pos_x += 0x14;
        }
        play_sound_at(obj, ((Slot04aObj *)obj)->side, 0x317);
        push_other_object_x_by_table(obj);
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
}

void update_sequence_then_launch_on_step_flag_slot04_01(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->vel_y = 0x30000;
        obj->accel_y = -0x6000;
        obj->state_3++;
        if (obj->facing_side == 0) {
            obj->vel_x = 0x38000;
        } else {
            obj->vel_x = -0x38000;
        }
    }
}
