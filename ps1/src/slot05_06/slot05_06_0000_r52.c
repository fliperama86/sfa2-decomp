/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);
void func_801312b8(Object *object);
extern ObjectFn data_801dd4f8_slot05_06[];

void func_801cc750_slot05_06(Object *obj) {
    if (((Slot04aObj *)obj)->field_3a != 0) {
        obj->field_07++;
        if (obj->field_129 == 0) {
            func_80140770(obj, 0x26, 5, 0x11, 0, 0, 1);
        } else {
            func_80140770(obj, 0x29, 5, 1, 0, 0, 1);
        }
    }
    func_80130efc(obj);
}

void func_801cc7d4_slot05_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        func_801312b8(obj);
    }
}

void func_801cc814_slot05_06(Object *obj) {
    int t = obj->field_4c;

    if (obj->field_0b == 0) {
        t = -t;
    }
    *(s32 *)&obj->field_10 += t;
    obj->field_4c += obj->field_54;
}

void func_801cc84c_slot05_06(Object *obj) {
    *(s32 *)&obj->field_14 -= obj->field_50;
    obj->field_50 += obj->field_58;
}

void func_801cc870_slot05_06(Object *obj, u16 *p) {
    if (obj->field_0b == 0) {
        *p = -*p;
    }
}

void func_801cc898_slot05_06(Object *obj) {
    data_801dd4f8_slot05_06[obj->field_06](obj);
}
