/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 **data_80018a6c_slot01[];
int func_800126f8_slot01(u8 index);
void func_800125f4_slot01(u8 *src, u8 unused);
void func_80012668_slot01(Object *obj);
void func_8001231c_slot01(Object *obj, u8 a);

void func_800121e0_slot01(Object *obj) {
    Object *o = obj->field_3c;
    u8 side;
    u8 **p = data_80018a6c_slot01[o->kind];
    p += o->field_d4;
    if (o->kind == 0x14) {
        func_80012668_slot01(obj);
    } else {
        func_800125f4_slot01(*p, func_800126f8_slot01(o->kind));
    }
    side = obj->field_48;
    obj->field_0c = 1;
    obj->field_09 = 3;
    obj->field_46 = 0x1f;
    obj->pos_x = 0xa0;
    obj->pos_y = 0x20;
    obj->field_5c = 0x70;
    obj->field_5e = 0x60;
    obj->field_4c = 0xc8000;
    obj->field_54 = -0xe758;
    obj->field_50 = 0xc0000;
    obj->field_58 = -0xa500;
    obj->field_0b = side;
    if (side != 0) {
        obj->pos_x = 0xc8;
        obj->field_5c = 0xf8;
        obj->field_4c = -obj->field_4c;
        obj->field_54 = -obj->field_54;
    }
    func_8001231c_slot01(obj, func_800126f8_slot01(o->kind));
}
