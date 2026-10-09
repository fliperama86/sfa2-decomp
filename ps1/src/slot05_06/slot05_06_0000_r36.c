/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd414_slot05_06[];
void func_80146998(Object *object);
void func_801cc814_slot05_06(Object *obj);

void func_801cae28_slot05_06(Object *obj) {
    data_801dd414_slot05_06[obj->field_07](obj);
}

void func_801cae68_slot05_06(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        func_80146998(obj);
        obj->field_07++;
    }
    func_80130efc(obj);
}

void func_801caebc_slot05_06(Object *obj) {
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

void func_801caf24_slot05_06(Object *obj) {
    u8 t = obj->field_3a;

    if (t != 0) {
        if (t != 2 && obj->field_4c >= 0) {
            func_801cc814_slot05_06(obj);
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
