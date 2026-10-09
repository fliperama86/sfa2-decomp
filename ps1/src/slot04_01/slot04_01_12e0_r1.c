/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1540_slot04_01(Object *obj);
void func_801b47cc_slot04_01(Object *obj, int kind);

void func_801b12e0_slot04_01(Object *obj) {
    func_801b1540_slot04_01(obj);
    if (obj->field_0b == 0) {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
    } else {
        *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
    }
    obj->field_4c = obj->field_4c + obj->field_54;
    if (obj->field_4c < 0) {
        obj->field_07++;
    }
    if (obj->field_12a == 4) {
        func_801b47cc_slot04_01(obj, 7);
    }
    func_80130efc(obj);
}

void func_801b1384_slot04_01(Object *o) {
    if (o->field_12a == 4) {
        if (o->field_50 < 0) {
            func_801b47cc_slot04_01(o, 8);
        } else {
            func_801b47cc_slot04_01(o, 7);
        }
    }
    func_801b1540_slot04_01(o);
    if (o->pos_y < o->field_70) {
        if (((Slot04aObj *)o)->field_3a != 0) {
            return;
        }
    } else {
        o->field_07++;
        o->field_45 = 0;
        o->field_159 = 0;
        o->field_17b = 0;
        o->pos_y = (u16)o->field_70;
        func_801209c4(o);
    }
    func_80130efc(o);
}
