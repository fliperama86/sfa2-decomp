/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b4e04_slot04_04[];
extern u8 data_801b7384_slot04_04[];
extern u32 data_801c40b0_slot04_04[];

void func_801b0000_slot04_04(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b4e04_slot04_04;
    obj->field_9c = data_801b7384_slot04_04;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x29; i++) {
        dst[i] = data_801c40b0_slot04_04[i];
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 4;
        c->field_03 = 0;
        c->field_3c = obj;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_0e = obj->field_0e;
        c->field_66 = obj->field_66;
        c->field_07 = obj->kind;
        *(u32 *)&obj->field_2c = (u32)c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 4;
        c->field_03 = 1;
        c->field_3c = obj;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_0e = obj->field_0e;
        c->field_07 = obj->kind;
        c->field_66 = obj->field_66;
        obj->field_28 = (u32)c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
    }
}
