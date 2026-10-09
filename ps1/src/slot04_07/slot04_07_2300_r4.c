/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b41c4_slot04_07(Object *obj);
void func_801b2994_slot04_07(Object *obj);

void func_801b279c_slot04_07(Object *obj) {
    s16 t;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t & 0x8000) {
        obj->field_07 = 5;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x4c);
    } else if (t != 0x3ff) {
        if ((t & 0xff) != 0) {
            obj->field_3a = t & 0xff00;
            obj->field_4c = 0x80000;
            obj->field_54 = -0x8000;
        }
        func_801b2994_slot04_07(obj);
    } else {
        obj->field_50 = 0x90000;
        obj->field_58 = 0xffff0000;
        obj->field_45 = 1;
        obj->field_07++;
        obj->field_54 = 0;
        if (obj->field_0b != 0) {
            obj->field_4c = 0xfffc8000;
        } else {
            obj->field_4c = 0x38000;
        }
    }
}

void func_801b2874_slot04_07(Object *obj) {
    if (func_801b41c4_slot04_07(obj) >= 0 || obj->pos_y < obj->field_70) {
        func_80130efc(obj);
    } else {
        obj->pos_y = obj->field_70;
        obj->field_14 = 0;
        obj->field_45 = 0;
        obj->field_07 = 5;
        func_801307e0(obj, (obj->field_12a >> 1) + 0x4c);
    }
}

void func_801b28f4_slot04_07(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_4c = 0xe0000;
        obj->field_54 = -0x6000;
        obj->field_07++;
    }
    func_80130efc(obj);
}
