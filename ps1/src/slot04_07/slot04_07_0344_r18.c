/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4404_slot04_07(Object *obj);
void func_801b48ec_slot04_07(Object *obj);
void func_801b4444_slot04_07(Object *obj);
void func_801b46cc_slot04_07(Object *obj);

void func_801b43c4_slot04_07(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b4404_slot04_07(obj);
    } else {
        func_801b48ec_slot04_07(obj);
    }
}

void func_801b4404_slot04_07(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b4444_slot04_07(obj);
    } else {
        func_801b46cc_slot04_07(obj);
    }
}
