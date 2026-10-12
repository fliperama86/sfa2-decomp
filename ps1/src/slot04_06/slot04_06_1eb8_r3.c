/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void move_x_by_vel_x_facing_then_accelerate_slot04_06(Object *obj);

void advance_state_3_on_mode_and_update_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->mode != 0) {
        o->state_3++;
        obj->mode = 0;
    }
    update_sequence(o);
}

void func_801b21a8_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->status_flags & 0x80) {
        o->other->other_state_code = 5;
        reset_object_to_state_1_0_sequence_0(o);
    } else {
        if (obj->mode != 0) {
            obj->mode = 0;
            o->active_flag_4 = 0;
        }
        if (o->vel_x >= 0) {
            move_x_by_vel_x_facing_then_accelerate_slot04_06(o);
        }
        start_action_if_buttons_pressed(o);
        update_sequence(o);
    }
}
