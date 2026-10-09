/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80146998(Object *object);

void func_801b14a0_slot04_01(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        obj->field_07 = 1;
        obj->field_4c = 0xb0000;
        obj->field_54 = -0x8000;
        obj->field_50 = 0xa0000;
        obj->field_58 = -0x6000;
        func_801204f4(obj, obj->side, 0xb);
        func_801307e0(obj, 0x30);
    } else {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            func_80146998(obj);
        }
        func_80130efc(obj);
    }
}

