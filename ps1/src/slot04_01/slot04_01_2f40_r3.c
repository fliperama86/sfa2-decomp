/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b38c4_slot04_01(Object *obj);
void func_801b31dc_slot04_01(Object *obj);
void func_801b35ec_slot04_01(Object *obj);

void func_801b317c_slot04_01(Object *obj) {
    if (obj->field_128 == 4) {
        func_801b38c4_slot04_01(obj);
    } else if (obj->field_129 != 0) {
        func_801b31dc_slot04_01(obj);
    } else {
        func_801b35ec_slot04_01(obj);
    }
}
