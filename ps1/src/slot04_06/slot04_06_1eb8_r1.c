/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1eb8_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    s16 t;

    *(s32 *)&o->field_10 += o->field_4c;
    o->field_4c += o->field_54;
    *(s32 *)&o->field_14 -= o->field_50;
    o->field_50 += o->field_58;
    if (o->pos_y >= (s16)obj->field_70) {
        o->field_07++;
        o->field_45 = 0;
        o->pos_y = obj->field_70;
        func_801307e0(o, 0x3b);
    } else {
        t = o->field_46;
        if (t == 0) {
            if (o->field_50 < 0) {
                o->field_46 = t + 1;
                o->field_6b = 0xa;
                o->field_50 = 0;
                o->field_58 = -0x6000;
                if (*(u16 *)&o->other->field_04 == 0x101) {
                    o->other->field_50 += 0x30000;
                }
                func_80130efc(o);
            }
        } else {
            func_80130efc(o);
        }
    }
}

void func_801b1fac_slot04_06(Object *obj) {
    s16 t;

    if ((s16)obj->field_3a >= 0) {
        t = obj->field_46;
        if (t == 0) {
            if (obj->field_50 < 0) {
                obj->field_46 = t + 1;
                obj->field_6b = 0xa;
                obj->field_50 = 0;
                obj->field_58 = -0x6000;
                if (*(u16 *)&obj->other->field_04 == 0x101) {
                    obj->other->field_50 += 0x30000;
                }
                func_80130efc(obj);
            }
        } else {
            func_80130efc(obj);
        }
    } else {
        func_80131468(obj);
    }
}
