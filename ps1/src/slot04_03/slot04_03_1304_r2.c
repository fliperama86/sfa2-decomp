/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80142adc(Object *object);

void func_801b13e0_slot04_03(Object *obj) {
    if ((u8)obj->field_3a == 0) {
        obj->field_45 = 1;
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        if (obj->field_49 != 0 && obj->field_50 > 0) {
            obj->field_58 = 0x10000;
        }
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 + obj->field_50;
        obj->field_50 = obj->field_50 + obj->field_58;
        if (obj->pos_y >= obj->field_70) {
            obj->field_07++;
            obj->field_45 = 0;
            obj->field_17b = 0;
            obj->pos_y = (u16)obj->field_70;
            func_801209c4(obj);
            func_801307e0(obj, 0x28);
            return;
        }
    }
    func_80130efc(obj);
}

void func_801b14b4_slot04_03(Object *obj) {
    func_80142adc(obj);
    if (!((s16)obj->field_3a & 0x8000)) {
        func_80130efc(obj);
    } else {
        obj->other->field_249 = 5;
        func_801312b8(obj);
    }
}
