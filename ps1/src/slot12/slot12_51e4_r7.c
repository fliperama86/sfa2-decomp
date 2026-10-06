/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8001606c_slot12(Object *obj, int a);
void func_80015dfc_slot12(Object *obj, int a);
void func_80015b90_slot12(Object *obj, int a);
void func_800159bc_slot12(Object *obj, int a);

void func_80015824_slot12(Object *obj) {
    if (game_state.field_2bc == 10) {
        obj->field_05 = 0;
        obj->field_04 += 1;
    }
}

void func_80015850_slot12(Object *obj) {
    func_8001606c_slot12(obj, 0);
    func_80015dfc_slot12(obj, 0);
    func_80015b90_slot12(obj, 0);
    if (game_state.field_2bd != 0) {
        func_800159bc_slot12(obj, 0);
    }
}

void func_800158b0_slot12(Object *obj) {
    func_8001606c_slot12(obj, 1);
    func_80015dfc_slot12(obj, 1);
    func_80015b90_slot12(obj, 1);
    if (game_state.field_2bd != 0) {
        func_800159bc_slot12(obj, 1);
    }
}
