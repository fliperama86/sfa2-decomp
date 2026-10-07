/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int arg);

int func_80140770(Object *object, u8 a, u8 b, s16 c, u16 d, u32 e, u8 f);

void func_801b20e0_slot04_08(Object *obj) {
    Object *p;
    obj->field_46 = (s16)obj->field_46 - 1;
    if (obj->field_46 & 0x8000) {
        obj->field_07 = obj->field_07 + 1;
        obj->field_0b = obj->field_0b ^ 1;
        p = obj->other;
        p->field_15b = 1;
        func_80140770(obj, 2, 0xf, -0x200, 0xf, 0, 0);
        if ((s16)p->field_5c < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x18;
            }
        }
        if (obj->field_0b != 0) {
            obj->field_4c = -0x18000;
        } else {
            obj->field_4c = 0x18000;
        }
        obj->field_50 = -0x50000;
        obj->field_58 = 0x5000;
        func_80130678(obj, 0x30);
    }
}

void func_801b21c8_slot04_08(Object *obj) {
    *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 + obj->field_50;
    obj->field_50 = obj->field_50 + obj->field_58;
    if (obj->field_70 < obj->pos_y) {
        obj->pos_y = obj->field_70;
        obj->field_45 = 0;
        *(s32 *)&obj->field_14 = *(s32 *)&obj->field_14 & 0xffff0000;
        func_801209c4(obj);
        obj->field_07 = obj->field_07 + 1;
        func_80130678(obj, 0x11);
    } else {
        func_80130efc(obj);
    }
}
