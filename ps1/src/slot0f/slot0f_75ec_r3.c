/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e80ec_slot0f(Object *obj, int a);
void func_800e7e8c_slot0f(Object *obj, int a);
void func_800e7c20_slot0f(Object *obj, int a);
void func_800e7a4c_slot0f(Object *obj, int a);

void func_800e78b4_slot0f(Object *obj) {
    if (game_state.field_2bc == 10) {
        obj->field_05 = 0;
        obj->field_04 += 1;
    }
}

void func_800e78e0_slot0f(Object *obj) {
    func_800e80ec_slot0f(obj, 0);
    func_800e7e8c_slot0f(obj, 0);
    func_800e7c20_slot0f(obj, 0);
    if (game_state.field_2bd != 0) {
        func_800e7a4c_slot0f(obj, 0);
    }
}

void func_800e7940_slot0f(Object *obj) {
    func_800e80ec_slot0f(obj, 1);
    func_800e7e8c_slot0f(obj, 1);
    func_800e7c20_slot0f(obj, 1);
    if (game_state.field_2bd != 0) {
        func_800e7a4c_slot0f(obj, 1);
    }
}
