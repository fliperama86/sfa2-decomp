/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_walk_or_restart_walk_sequence_with_speed(Object *object) {
    int index;
    s32 speed;
    if ((u8)is_facing_differs_from_saved(object) == 0) {
        if (object->direction_flag == object->variant_index) {
            *(s32 *)&object->pos_x_frac += object->vel_x;
            update_sequence(object);
            return;
        }
        object->variant_index = object->direction_flag;
    }
    object->facing_side = object->saved_facing;
    index = 2;
    if (object->variant_index != 0) {
        index = 3;
    }
    set_sequence_by_side(object, index);
    speed = motion_speed_table[object->kind * 2 + (object->variant_index & 1)];
    if (object->facing_side != 0) {
        speed = -speed;
    }
    object->vel_x = speed;
    choose_idle_state_transition_or_update(object);
}
