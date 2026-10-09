/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int arg);
void func_801b359c_slot04_02(Object *obj);
void func_8013788c(Object *object);
void func_80142adc(Object *object);

void func_801b33fc_slot04_02(Object *obj) {
    int t;

    if (obj->field_7e == 0) {
        func_801b359c_slot04_02(obj);
    }
    if (obj->field_48 != 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    obj->field_0b = obj->field_158;
    if (((obj->field_164 >> obj->field_48) & 1) == 0) {
        t = 0x30000;
        if (obj->field_129 != 0) {
            t = 0x24000;
        }
        if (t < obj->field_4c) {
            func_80130efc(obj);
            return;
        }
    }
    obj->field_07 = obj->field_07 + 1;
    obj->field_17b = 0;
    obj->field_46 = *(u8 *)&obj->field_46 | 0xa00;
    func_80130678(obj, 0);
}

void func_801b34fc_slot04_02(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->field_0b = obj->field_158;
        obj->other->field_249 = 5;
        func_801312b8(obj);
        if (obj->field_7e == 0) {
            func_8013788c(obj);
        }
    } else {
        if (obj->field_7e == 0) {
            func_801b359c_slot04_02(obj);
        }
        func_80142adc(obj);
        func_80130efc(obj);
    }
}
