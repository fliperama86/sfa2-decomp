/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801dd5b0_slot05_06[])(Object *);
extern Object *data_801dd754_slot05_06;
void func_801cdc58_slot05_06(Object *obj);

void func_801cdac4_slot05_06(Object *obj) {
    u16 t;

    obj->field_07 = 3;
    obj->field_159 = 1;
    func_80141f28(obj, obj->field_12a >> 1);
    t = 0x12;
    if (obj->field_48 == 0) {
        t = 0xc;
    }
    if (obj->field_129 != 0) {
        t += 3;
    }
    t = (obj->field_12a >> 1) + t;
    func_801307e0(obj, t);
}

void func_801cdb44_slot05_06(Object *obj) {
    func_8011f14c((Slab172 *)obj);
}

void func_801cdb64_slot05_06(Object *obj) {
    data_801dd754_slot05_06 = obj->field_3c;
    data_801dd5b0_slot05_06[obj->field_04](obj);
}

void func_801cdbb4_slot05_06(Object *obj) {
    Object *p;

    obj->field_04++;
    p = data_801dd754_slot05_06;
    obj->field_1c = p->field_1c;
    obj->field_1e = p->field_1e;
    obj->field_03 = p->kind;
    obj->field_0c = data_801dd754_slot05_06->field_0c;
    obj->field_0d = data_801dd754_slot05_06->field_0d;
    obj->field_0e = data_801dd754_slot05_06->field_0e;
    obj->field_48 = 0;
    func_801cdc58_slot05_06(obj);
}
