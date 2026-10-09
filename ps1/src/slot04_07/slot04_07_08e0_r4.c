/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0bd8_slot04_07(Object *obj);
void func_801b0cd0_slot04_07(Object *obj);

void func_801b0b98_slot04_07(Object *obj) {
    if (obj->field_129 == 0) {
        func_801b0bd8_slot04_07(obj);
    } else {
        func_801b0cd0_slot04_07(obj);
    }
}
