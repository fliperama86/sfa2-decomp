/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b05a8_slot04_04(Object *obj);
void func_801b042c_slot04_04(Object *obj);
void func_801b0a40_slot04_04(Object *obj);

void func_801b03d4_slot04_04(Object *obj) {
    if (obj->field_128 == 4) {
        func_801b0a40_slot04_04(obj);
    } else if (obj->field_128 != 0) {
        func_801b05a8_slot04_04(obj);
    } else {
        func_801b042c_slot04_04(obj);
    }
}
