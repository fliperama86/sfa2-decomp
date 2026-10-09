/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b34f8_slot04_05(Object *object);
void func_801b36f4_slot04_05(Object *object);

void func_801b34b8_slot04_05(Object *obj) {
    if (obj->field_129 != 0) {
        func_801b36f4_slot04_05(obj);
    } else {
        func_801b34f8_slot04_05(obj);
    }
}
