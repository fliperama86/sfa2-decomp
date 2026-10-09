/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int arg);

void func_801b01a0_slot04_05(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b01d4_slot04_05(Object *obj) {
    if (game_state.field_64 == 0 && game_state.field_5c == 0) {
        obj->field_06 = obj->field_06 + 1;
    }
    func_80130efc(obj);
}
