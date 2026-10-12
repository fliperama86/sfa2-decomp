/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void stop_x_motion_against_facing_then_move_slot04_09(Object *obj);

void update_sequence_spawn_children_or_start_jump_slot04_09(Object *obj) {
    u16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if ((u8)t == 0) {
        obj->active_state = 1;
        obj->state_3++;
        obj->other_lock = 0;
        if (obj->field_165 & 0x80) {
            obj->other->field_6b = 10;
            obj->other_lock = 5;
        }
        obj->field_165 = 0;
        obj->vel_x = 0x80000;
        obj->accel_x = -0x8000;
        obj->vel_y = 0x80000;
        obj->accel_y = -0x6000;
        if (obj->facing_side == 0) {
            obj->vel_x = -0x80000;
            obj->accel_x = 0x8000;
        }
        set_sequence_by_side_and_mode(obj, 0x33);
    } else if ((u8)t != 2) {
        obj->step_flags = (t & 0xff00) | 2;
        obj->field_165 = 0xff;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        }
        play_sound_at(obj, obj->side, 0x31c);
        spawn_children_per_direction_index(obj, -0x1f, 0x2e);
    }
}

void advance_state_3_and_update_moving_slot04_09(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    stop_x_motion_against_facing_then_move_slot04_09(obj);
    update_sequence(obj);
}
