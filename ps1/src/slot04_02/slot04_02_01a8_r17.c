/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);
void func_80146998(Object *object);

void func_801b1bc0_slot04_02(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->other->field_249 = 5;
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 & 0xffff0000;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}

void func_801b1c30_slot04_02(Object *obj) {
    s16 t = obj->field_3a;

    if (t & 0x8000) {
        obj->field_07 = 1;
        obj->field_4c = 0x80000;
        obj->field_54 = -0x8000;
        obj->field_50 = 0x90000;
        obj->field_58 = -0x6000;
        func_801307e0(obj, 0x60);
    } else {
        if (t & 0xff) {
            obj->field_3a = t & 0xff00;
            func_80146998(obj);
        }
        func_80130efc(obj);
    }
}

void func_801b1cc0_slot04_02(Object *obj) {
    if (obj->field_49 != 0 && obj->field_50 < 0) {
        obj->field_58 = 0xffff0000;
    }
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 - obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
}
