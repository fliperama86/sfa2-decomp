/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801e1fdc_slot0b[])(Object *);
extern u8 data_801e766c_slot0b[];
void func_801e01dc_slot0b(Object *obj);
void func_801e101c_slot0b(Object *obj, u8 *a, int b, int c);
int func_8015bdd4(int a, int b);

void func_801e00c4_slot0b(Object *obj) {
    ref_other.p = obj->field_3c;
    data_801e1fdc_slot0b[obj->field_04](obj);
}

void func_801e0114_slot0b(Object *obj) {
    int s;
    int r;

    obj->field_0b = 0;
    obj->field_0e = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_24 = 0;
    obj->field_04 = obj->field_04 + 1;
    if (obj->field_03 == 0) {
        obj->field_01 = 0;
        obj->field_0c = 0;
        obj->field_09 = 3;
        func_80130768(obj, 0, (SequenceStep **)obj->field_58);
        s = func_8015bd0c(0, 0, 0x280, 0) & 0xffff;
        r = func_8015bdd4(0x140, 0x1e6);
        func_801e101c_slot0b(obj, data_801e766c_slot0b + obj->field_48 * 0x1e0, s, r & 0xffff);
        func_801e01dc_slot0b(obj);
    }
}
