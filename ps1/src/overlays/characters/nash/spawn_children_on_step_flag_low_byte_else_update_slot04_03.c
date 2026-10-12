/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void spawn_children_on_step_flag_low_byte_else_update_slot04_03(Object *obj) {
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags &= 0xff00;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, 0xf, 0x4e);
    } else {
        update_sequence(obj);
    }
}

void release_other_lock_on_step_flag_slot04_03(Object *obj) {
    update_sequence(obj);
    if ((u8)obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags &= 0xff00;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
        }
        obj->other_lock = 0;
    }
}

void set_vel_x_by_facing_on_step_flag_slot04_03(Object *obj) {
    if ((u8)obj->step_flags == 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->step_flags &= 0xff00;
        if (obj->facing_side != 0) {
            obj->vel_x = 0x44000;
        } else {
            obj->vel_x = -0x44000;
        }
    }
}
