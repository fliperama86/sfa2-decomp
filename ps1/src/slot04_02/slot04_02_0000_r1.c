/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b7f84_slot04_02[];
extern u8 data_801ba32c_slot04_02[];
extern u32 data_801c60f8_slot04_02[];

void func_801b0000_slot04_02(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;
    u8 s;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b7f84_slot04_02;
    obj->field_9c = data_801ba32c_slot04_02;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c60f8_slot04_02[i];
    }
    obj->field_252 = 0;
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 2;
        c->field_3c = obj;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_0e = obj->field_0e;
        s = obj->side;
        obj->field_28 = (u32)c;
        c->field_66 = s;
        c->field_0d = obj->field_0d;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
}
