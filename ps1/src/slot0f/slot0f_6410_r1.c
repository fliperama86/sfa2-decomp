/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u32 data_800ebcb8_slot0f[];
extern SequenceStep *data_800ef8d0_slot0f[];
extern void (*data_800efd8c_slot0f[])(Object *);

void func_800e6410_slot0f(Object *obj, int a) {
    char *base = (char *)0x80061000;
    obj->field_90 = base;
    obj->field_90 = base + data_800ebcb8_slot0f[obj->field_60];
    func_80130768(obj, (a + obj->field_60 * 2) & 0xff, data_800ef8d0_slot0f);
}

void func_800e6470_slot0f(Object *obj) {
    data_800efd8c_slot0f[obj->field_04](obj);
}
