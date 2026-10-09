/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int index);
void func_80126130(void);
u8 func_8013f8c4(Object *obj, int a, int b);

void func_801b3ef8_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *p = o->other;

    o->field_46 = (s16)o->field_46 - 1;
    if ((o->field_46 & 0xff) != 0) {
        *(s32 *)&o->field_10 += o->field_4c;
        if (((o->field_164 >> o->field_48) & 1) == 0) {
            if (func_8013f8c4(o, -0x10, 0x10) == 0) {
                func_80130efc(o);
            } else {
                o->field_07++;
                o->field_46 = (o->field_46 & 0xff00) | 0x10;
                func_80120554(p, p->side, 0x31a);
                func_801307e0(o, 0x3b);
            }
        } else {
            o->field_07 = 8;
            o->field_46 = obj->field_46 | 0x800;
            func_80130678(o, 0);
        }
    } else {
        o->field_07 = 8;
        o->field_46 = obj->field_46 | 0x800;
        func_80130678(o, 0);
    }
}

void func_801b3fec_slot04_02(Object *o) {
    o->field_46 = (s16)o->field_46 - 1;
    if ((o->field_46 & 0xff) == 0) {
        o->field_07++;
        o->field_46 = o->field_46 | 0x3b;
        func_80126130();
        func_80130efc(o);
    }
}
