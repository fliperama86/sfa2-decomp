/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3734_slot04_00(Object *obj);
void func_801b3c38_slot04_00(Object *obj);
void func_801b3774_slot04_00(Object *obj);
void func_801b3988_slot04_00(Object *obj);

void func_801b36f4_slot04_00(Object *obj) {
    if (obj->field_128 == 0) {
        func_801b3734_slot04_00(obj);
    } else {
        func_801b3c38_slot04_00(obj);
    }
}

void func_801b3734_slot04_00(Object *obj) {
    obj->field_157 = 0;
    if (obj->field_129 == 0) {
        func_801b3774_slot04_00(obj);
    } else {
        func_801b3988_slot04_00(obj);
    }
}
