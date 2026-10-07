/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b29c8_slot04_01(Object *obj) {
    int a = 0x3e;

    obj->field_17b = 1;
    obj->field_07++;
    if (obj->field_49 == 0) {
        obj->field_177--;
    }
    obj->field_159 = 0;
    obj->field_157 = 0;
    if (obj->field_130 & 0x8000) {
        a = 0x3f;
    }
    func_801307e0(obj, a);
}

void func_801b2a30_slot04_01(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            func_801204f4(obj, obj->side, 9);
        }
        func_80130efc(obj);
    }
}
