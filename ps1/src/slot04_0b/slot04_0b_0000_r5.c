/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b0a38_slot04_0b(Object *obj) {
    Object *o = obj;
    if (*(u8 *)&obj->field_3a == 0) {
        o->field_07 = obj->field_07 + 1;
        func_80142a14(o);
    } else {
        if (obj->field_134 & 0x20) {
            obj->field_07 = obj->field_07 + 1;
            func_801307e0(obj, 0x49);
        }
        func_80142a14(obj);
    }
}
