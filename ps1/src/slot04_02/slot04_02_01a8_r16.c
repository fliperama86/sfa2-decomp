/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1cc0_slot04_02(Object *obj);

void func_801b1a3c_slot04_02(Object *o) {
    if (*(u8 *)&o->field_3a == 0) {
        o->field_45 = 1;
        o->field_07++;
    }
    func_80130efc(o);
}

void func_801b1a7c_slot04_02(Object *obj) {
    func_801b1cc0_slot04_02(obj);
    if (obj->pos_y < obj->field_70) {
        if (obj->field_0b != 0) {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 + obj->field_4c;
        } else {
            *(s32 *)&obj->field_10 = *(s32 *)&obj->field_10 - obj->field_4c;
        }
        obj->field_4c = obj->field_4c + obj->field_54;
        if (obj->field_4c < 0) {
            obj->field_07++;
        }
    } else {
        obj->field_17b = 0;
        obj->field_49 = 0;
        obj->field_45 = 0;
        obj->pos_y = (u16)obj->field_70;
        func_801209c4(obj);
    }
    func_80130efc(obj);
}

void func_801b1b3c_slot04_02(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    s16 y;

    func_801b1cc0_slot04_02(o);
    y = o->field_70;
    if (o->pos_y < y) {
        if (obj->field_3a != 0) {
            return;
        }
    } else {
        o->field_07++;
        o->pos_y = y;
        o->field_17b = 0;
        o->field_159 = 0;
        o->field_45 = 0;
        func_801209c4(o);
    }
    func_80130efc(o);
}
