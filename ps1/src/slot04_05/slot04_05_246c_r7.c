/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b3048_slot04_05(Object *object);

void func_801b2e04_slot04_05(Object *obj) {
    u8 t;
    func_80130efc(obj);
    t = obj->field_3a;
    if (t == 2) {
        obj->field_4c = 0xa0000;
        obj->field_07++;
    } else if (t != 0) {
        if (obj->field_4c >= 0) {
            func_801b3048_slot04_05(obj);
        }
    }
}

void func_801b2e74_slot04_05(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    s16 t = o->field_3a;
    if (t & 0x8000) {
        o->field_07++;
        if (o->field_0b != 0) {
            o->field_4c = 0x40000;
        } else {
            o->field_4c = 0xfffc0000;
        }
        o->field_50 = 0x90000;
        o->field_58 = -0x7000;
        o->field_54 = 0;
        o->field_45 = 1;
        func_801307e0(o, 0x30);
    } else {
        if ((u8)t != 0) {
            switch ((u8)t) {
            default:
            dflt:
                if (o->field_4c >= 0) {
                    func_801b3048_slot04_05(o);
                }
                break;
            case 3:
                o->field_4c = 0x40000;
                o->field_54 = 0xfffff000;
                break;
            case 4:
                goto c4;
            }
        }
        func_80130efc(o);
        return;
    c4:
        if (o->field_cd != 0) {
            obj->field_1a6 = o->field_129;
        }
        if (obj->field_1a6 == 0) {
            goto dflt;
        }
        obj->field_1a5 = 0x18;
        o->field_07 += 3;
    }
}
