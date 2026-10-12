/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e6410_slot0f(Object *obj, int a);

void func_800e62dc_slot0f(Object *obj) {
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

void func_800e6348_slot0f(Object *obj) {
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

void func_800e63b0_slot0f(Object *obj) {
    if (game_state.field_2bc == 10) {
        obj->state_0++;
    }
    advance_sequence_checked(obj);
}

void func_800e63f0_slot0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
