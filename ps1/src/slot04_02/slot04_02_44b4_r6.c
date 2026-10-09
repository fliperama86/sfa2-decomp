/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4d28_slot04_02(Object *obj);

void func_801b5184_slot04_02(Object *obj) {
    if (obj->field_0b != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_07 = obj->field_07 + 1;
        func_801307e0(obj, 0x56);
        func_80142adc(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
        func_801b4d28_slot04_02(obj);
    }
}

void func_801b5234_slot04_02(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_157 = 0;
        obj->field_45 = 0;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}
