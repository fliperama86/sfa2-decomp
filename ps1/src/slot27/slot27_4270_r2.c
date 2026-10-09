/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80027c6c_slot27[];
extern ObjectFn data_80027c7c_slot27[];
extern ObjectFn data_80027cac_slot27[];
void func_80014c1c_slot27(Object *obj);
void func_80014cd8_slot27(Object *obj);

void func_800143ac_slot27(Object *obj) {
    data_80027c6c_slot27[obj->field_04](obj);
}

void func_800143ec_slot27(Object *obj) {
    obj->field_04 = obj->field_04 + 1;
    func_80014c1c_slot27(obj);
    func_80014cd8_slot27(obj);
}

void func_8001442c_slot27(Object *obj) {
    data_80027c7c_slot27[obj->field_03](obj);
    func_80131094(obj);
}

void func_80014480_slot27(Object *obj) {
    data_80027cac_slot27[obj->field_05](obj);
}

void func_800144c0_slot27(Object *obj) {
    obj->field_7a = 0x20;
    obj->field_7c = 0x1e0;
    obj->field_01 = 1;
    obj->field_05 = obj->field_05 + 1;
    if (obj->field_03 == 10) {
        obj->field_4c = 8;
    } else {
        obj->field_4c = -8;
    }
}
