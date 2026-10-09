/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

s32 func_801b3a84_slot04_04(Object *object);
void func_80142adc(Object *object);

void func_801b230c_slot04_04(Object *obj) {
    if (func_801b3a84_slot04_04(obj) >= 0) {
        func_80130efc(obj);
    } else {
        if (obj->field_49 != 0) {
            obj->field_58 = 0xffff0000;
        } else {
            obj->field_58 = -0x6000;
        }
        if (obj->pos_y < obj->field_70) {
            func_80130efc(obj);
        } else {
            obj->field_07++;
            obj->field_14 = 0;
            obj->field_45 = 0;
            obj->field_159 = 0;
            obj->field_17b = 0;
            obj->pos_y = (u16)obj->field_70;
            func_801209c4(obj);
            func_801307e0(obj, obj->field_12a + 0x31);
        }
    }
}

void func_801b23b4_slot04_04(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        func_80130efc(obj);
    }
}
