/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);
void func_80146998(Object *object);

void func_801b2e68_slot04_06(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        func_80146998(obj);
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801b2ebc_slot04_06(Object *obj) {
    if ((s16)obj->field_3a >= 0) {
        func_80130efc(obj);
    } else {
        obj->field_4c = 0xc0000;
        obj->field_12c = 0;
        obj->field_12d = 0;
        obj->field_12e = 0;
        obj->field_12f = 0;
        obj->field_54 = 0xffff0000;
        obj->field_07++;
        func_801307e0(obj, 0x3a);
    }
}

void func_801b2f24_slot04_06(Object *obj) {
    u8 t = obj->field_3a;

    if (t != 0) {
        if (t != 2 && obj->field_4c >= 0) {
            func_801b4818_slot04_06(obj);
            func_80130efc(obj);
        } else {
            obj->field_54 = -0x6000;
            obj->field_07++;
            func_80130efc(obj);
        }
    } else {
        func_80130efc(obj);
    }
}

void func_801b2fa8_slot04_06(Object *obj) {
    if (obj->field_4c >= 0) {
        func_801b4818_slot04_06(obj);
        func_80130efc(obj);
    } else {
        obj->field_07++;
        func_80130efc(obj);
    }
}
