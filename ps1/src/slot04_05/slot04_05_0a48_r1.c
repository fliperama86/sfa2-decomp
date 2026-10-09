/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0a48_slot04_05(Object *obj) {
    obj->field_07++;
    if (obj->field_12a == 2 && obj->field_25f == 0 && (obj->field_130 & 0x8000) != 0) {
        obj->field_159 = 1;
        obj->field_07 = 2;
        func_80130ec0(obj);
        func_801307e0(obj, 0x28);
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}
