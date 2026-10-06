/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80014dd0_slot12(Object *obj, int a);

void func_80014c24_slot12(Object *obj) {
    int t = obj->field_46 - 1;
    obj->field_46 = t;
    if ((s16)t < 0) {
        obj->field_05++;
        game_state.field_2bc = 5;
    }
    func_80131094(obj);
}

void func_80014c7c_slot12(Object *obj) {
    if (game_state.field_2bc == 8) {
        obj->field_46 = 10;
        obj->field_05++;
        if ((obj->field_60 == 2) | (obj->field_60 == 0x14)) {
            goto second;
        }
        func_80014dd0_slot12(obj, 1);
    second:
        func_80014dd0_slot12(obj, 1);
    } else {
        func_80131094(obj);
    }
}
