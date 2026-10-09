/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80130184(Object *object);
int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_80145f98(Object *object);
void func_801b1078_slot04_10(Object *object);
void func_801b4ec0_slot04_10(Object *object);

void func_801b4d84_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 0;
        obj->field_45 = 1;
        func_801b4ec0_slot04_10(obj);
        if (obj->field_50 < 0) {
            obj->field_07++;
            obj->field_4c = 0;
            obj->field_54 = 0;
            func_801307e0(obj, 0x32);
        } else if (func_8013ffe4(obj, -0x3e, 0x18, 0x40, 0x1a) != 0) {
            func_80120554(obj, obj->side ^ 1, 0x31a);
            func_80145f98(obj);
            if (obj->field_12a == 4) {
                func_801204f4(obj, obj->side, 7);
                obj->field_07 = 0xe;
                func_801307e0(obj, 0x37);
            } else if (obj->field_12a == 2) {
                obj->field_07 = 9;
                func_801307e0(obj, 0x34);
            } else {
                obj->field_07 = 6;
                func_801307e0(obj, 0x31);
            }
        }
    }
}

void func_801b4ec0_slot04_10(Object *obj) {
    if (obj->field_4c >= 0) {
        if (obj->field_0b == 0) {
            obj->field_4c = 0;
            obj->field_54 = 0;
        }
    } else if (obj->field_0b != 0) {
        obj->field_4c = 0;
        obj->field_54 = 0;
    }
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
    *(s32 *)&obj->field_10 += obj->field_4c;
    obj->field_4c += obj->field_54;
}

void func_801b4f48_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        obj->field_45 = 0;
        obj->field_159 = 0;
        obj->pos_y = obj->field_70;
        *(s32 *)&obj->field_14 &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x33);
    }
}

void func_801b4fc8_slot04_10(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b1078_slot04_10(obj);
    }
}
