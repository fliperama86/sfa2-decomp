/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b487c_slot04_03[];
extern u8 data_801b661c_slot04_03[];
extern u32 data_801c0460_slot04_03[];

Object *func_8011f32c(void);

void func_801b0000_slot04_03(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b487c_slot04_03;
    obj->field_9c = data_801b661c_slot04_03;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c0460_slot04_03[i];
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 3;
        c->field_03 = obj->kind;
        c->field_3c = obj;
        obj->field_28 = (u32)c;
        c->field_66 = obj->field_66;
        c->field_0d = obj->field_0d;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
    }
}
