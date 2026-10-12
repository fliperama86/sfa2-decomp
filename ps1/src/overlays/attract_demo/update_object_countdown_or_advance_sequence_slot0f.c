/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void func_800e6410_slot0f(Object *obj, int a);

void update_object_countdown_or_advance_sequence_slot0f(Object *obj) {
    if (game_state.field_2bc == 11) {
        int t = obj->countdown - 1;
        obj->countdown = t;
        if ((s16)t < 0) {
            obj->countdown = 10;
            obj->state_1++;
            func_800e6410_slot0f(obj, 1);
            return;
        }
    }
    advance_sequence_checked(obj);
}

void update_countdown_then_advance_sequence_slot0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        int t = obj->countdown - 1;
        obj->countdown = t;
        if ((s16)t < 0) {
            obj->state_1++;
            game_state.field_2bc = 9;
        }
    }
    advance_sequence_checked(obj);
}

void advance_state_0_on_2bc_10_and_sequence_slot0f(Object *obj) {
    if (game_state.field_2bc == 10) {
        obj->state_0++;
    }
    advance_sequence_checked(obj);
}

void release_object_record_slot0f(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
