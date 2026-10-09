/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80019330_slot01[];
extern u8 data_800195b8_slot01[];
extern u8 data_800319b8_slot01[];
extern SequenceStep *data_800212ac_slot01[];
Poly28 *func_80012f60_slot01(Object *obj, Poly28 *p, int a2, int a3);
void func_80012d08_slot01(Object *obj);

void func_80012b90_slot01(Object *obj) {
    Object *r;

    obj->field_7a = 0x70;
    obj->field_7c = 0x1ea;
    obj->field_98 = data_80019330_slot01;
    obj->field_9c = data_800195b8_slot01;
    obj->field_90 = (void *)0x80059000;
    obj->field_0b = 0;
    obj->field_0e = 0;
    obj->field_20 = 0;
    obj->field_22 = 0;
    obj->field_24 = 0;
    obj->field_04 = obj->field_04 + 1;
    r = obj->field_3c;
    obj->pos_x = r->pos_x;
    obj->pos_y = r->pos_y;
    if (obj->field_03 & 0x80) {
        obj->field_01 = 1;
        obj->field_0c = 1;
        obj->field_09 = 0;
        func_80130768(obj, obj->field_03 - 0x80, data_800212ac_slot01);
    } else {
        obj->field_01 = 0;
        obj->field_0c = 0;
        obj->field_09 = 3;
        func_80130768(obj, 0, (SequenceStep **)obj->field_58);
        func_80012f60_slot01(obj, (Poly28 *)data_800319b8_slot01, (u16)func_8015bd0c(0, 0, 0x300, 0), (u16)func_8015bdd4(0x70, 0x1eb));
        func_80012f60_slot01(obj, (Poly28 *)(data_800319b8_slot01 + 0xf0), (u16)func_8015bd0c(0, 0, 0x300, 0), (u16)func_8015bdd4(0x70, 0x1eb));
        func_80012d08_slot01(obj);
    }
}
