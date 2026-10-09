/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"
void func_80146998(Object *object);

void func_801b1b44_slot04_05(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        obj->field_07 = 1;
        obj->field_4c = 0xa0000;
        obj->field_54 = -0x8000;
        obj->field_50 = 0x80000;
        obj->field_58 = -0x6000;
        func_801307e0(obj, 0x1b);
    } else {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            func_80146998(obj);
            obj->other->field_6b = 0x14;
        }
        func_80130efc(obj);
    }
}

void func_801b1be0_slot04_05(Object *obj) {
    if (obj->field_49 != 0 && (obj->field_50 & 0x8000)) {
        obj->field_58 = 0xffff0000;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
}
