/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4280_slot04_05(Object *obj) {
    obj->field_0b = obj->field_158;
    obj->field_07++;
    if (obj->field_12a == 2) {
        if (obj->field_219 != 0) {
            obj->field_159 = 1;
            obj->field_07 = 2;
            func_80141f28(obj, 1);
            func_801307e0(obj, 0x28);
        } else {
            obj->field_159 = 1;
            func_80130dc0(obj);
        }
    } else {
        obj->field_159 = 1;
        func_80130dc0(obj);
    }
}

void func_801b4310_slot04_05(Object *obj) {
    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->field_07 = 0;
        }
        func_80130efc(obj);
    }
}
