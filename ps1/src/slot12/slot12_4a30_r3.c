/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80014dd0_slot12(Object *obj, int a);

void func_80014c24_slot12(Object *obj) {
    int t = obj->countdown - 1;
    obj->countdown = t;
    if ((s16)t < 0) {
        obj->state_1++;
        game_state.field_2bc = 5;
    }
    advance_sequence_checked(obj);
}

void func_80014c7c_slot12(Object *obj) {
    if (game_state.field_2bc == 8) {
        obj->countdown = 10;
        obj->state_1++;
        if ((obj->field_60 == 2) | (obj->field_60 == 0x14)) {
            goto second;
        }
        func_80014dd0_slot12(obj, 1);
    second:
        func_80014dd0_slot12(obj, 1);
    } else {
        advance_sequence_checked(obj);
    }
}
