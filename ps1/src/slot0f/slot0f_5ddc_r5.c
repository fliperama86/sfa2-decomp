/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e6410_slot0f(Object *obj, int a);

void func_800e62dc_slot0f(Object *obj) {
    if (game_state.field_2bc == 11) {
        int t = obj->field_46 - 1;
        obj->field_46 = t;
        if ((s16)t < 0) {
            obj->field_46 = 10;
            obj->field_05++;
            func_800e6410_slot0f(obj, 1);
            return;
        }
    }
    func_80131094(obj);
}

void func_800e6348_slot0f(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        int t = obj->field_46 - 1;
        obj->field_46 = t;
        if ((s16)t < 0) {
            obj->field_05++;
            game_state.field_2bc = 9;
        }
    }
    func_80131094(obj);
}

void func_800e63b0_slot0f(Object *obj) {
    if (game_state.field_2bc == 10) {
        obj->field_04++;
    }
    func_80131094(obj);
}

void func_800e63f0_slot0f(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
