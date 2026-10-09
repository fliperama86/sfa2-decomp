/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *object);
extern ObjectFn data_801dd4ac_slot05_06[];

void func_801cbe80_slot05_06(Object *obj) {
    if (*(u8 *)&obj->field_3a != 0) {
        obj->field_07++;
        func_80146998(obj);
    }
    func_80130efc(obj);
}

void func_801cbed0_slot05_06(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_07++;
        obj->field_4c = 0xa0000;
        obj->field_54 = -0xc000;
        func_801307e0(obj, 0x1a);
    } else {
        func_80130efc(obj);
    }
}

void func_801cbf2c_slot05_06(Object *obj) {
    s16 t = obj->field_3a;

    if (t < 0) {
        func_80131468(obj);
    } else {
        if (t == 3) {
            func_80120554(obj, obj->side, 0x324);
        }
        if ((s16)obj->field_3a == 2 && obj->field_4c >= 0) {
            func_801cc814_slot05_06(obj);
        }
        func_80130efc(obj);
    }
}

void func_801cbfb4_slot05_06(Object *obj) {
    data_801dd4ac_slot05_06[obj->field_07](obj);
}
