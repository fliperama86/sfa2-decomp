/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, u16 arg);

void func_801b3778_slot04_0d(Object *object) {
    object->field_06 = object->field_06 + 1;
    object->field_0b = object->field_158;
    func_80130678(object, 0);
}

void func_801b37ac_slot04_0d(Object *object) {
    if (game_state.field_5c == 0) {
        object->field_06 = object->field_06 + 1;
    }
    func_80130efc(object);
}
