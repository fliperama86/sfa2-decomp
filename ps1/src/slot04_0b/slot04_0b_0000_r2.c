/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 move_ballistic_and_check_ground_slot04_0b(Object *obj);

void inc_state_3_side_sequence_forty_eight_slot04_0b(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    set_sequence_by_side(obj, 0x30);
}

void func_801b03a8_slot04_0b(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_3 = obj->state_3 + 1;
        set_sequence_by_side(obj, 0x10);
    } else {
        update_sequence(obj);
    }
}

void func_801b03f8_slot04_0b(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->vel_y = 0x80000;
        obj->accel_x = 0;
        obj->accel_y = -0x9000;
        obj->state_3 = obj->state_3 + 1;
        if (obj->facing_side == 0) {
            obj->vel_x = 0x28000;
        } else {
            obj->vel_x = -0x28000;
        }
        obj->active_state = 1;
        set_sequence_by_side(obj, 0x14);
    } else {
        update_sequence(obj);
    }
}

void move_ballistic_then_land_on_ground_slot04_0b(Object *obj) {
    if (move_ballistic_and_check_ground_slot04_0b(obj) == 0) {
        obj->active_state = 0;
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        play_object_sound_by_kind(obj);
        set_sequence_by_side(obj, 0x11);
    } else {
        update_sequence(obj);
    }
}
