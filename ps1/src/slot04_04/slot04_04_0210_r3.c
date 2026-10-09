/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b05e8_slot04_04(Object *obj);
void func_801b0634_slot04_04(Object *obj);

void func_801b05a8_slot04_04(Object *obj) {
    obj->field_157 = 1;
    if (obj->field_129 != 0) {
        func_801b0634_slot04_04(obj);
    } else {
        func_801b05e8_slot04_04(obj);
    }
}
