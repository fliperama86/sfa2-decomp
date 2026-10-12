/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

int move_by_velocity_return_falling_slot04_01(Object *obj);

void update_airborne_fall_then_hit_other_slot04_01(Object *obj) {
    if (move_by_velocity_return_falling_slot04_01(obj)) {
        if (obj->ground_y <= obj->pos_y) {
            obj->pos_y = obj->ground_y;
            game_state.state_param = 0x18;
        }
    }
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 5, 0x11, 0, 1, 0);
    }
    update_sequence(obj);
}

void check_landing_set_state_param_slot04_01(Object *obj) {
    if (move_by_velocity_return_falling_slot04_01(obj)) {
        if (obj->ground_y <= obj->pos_y) {
            obj->pos_y = obj->ground_y;
            game_state.state_param = 0x18;
            obj->state_3 = obj->state_3 + 1;
        }
    }
}

void update_sequence_or_flip_facing_and_reset_b_slot04_01(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->facing_side = obj->facing_side ^ 1;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
