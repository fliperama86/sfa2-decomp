/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800172d0_slot12(Object *o);
void func_80017334_slot12(Object *o);

void func_800171cc_slot12(Object *obj) {
    if (game_state.field_2bc == 5) {
        obj->field_05++;
    }
}

void func_800171f8_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    obj->field_12--;
    obj->field_4c--;
    if (obj->field_4c == 0) {
        obj->field_05++;
    }
    func_800172d0_slot12(o);
    func_80017334_slot12(o);
}

void func_80017258_slot12(Object *obj) {
    obj->field_05++;
    game_state.field_2bc = 3;
}

void func_8001727c_slot12(Object *obj) {
    func_80017334_slot12(obj);
    func_800172d0_slot12(obj);
    if (game_state.field_2bc == 10) {
        obj->field_04++;
    }
}
