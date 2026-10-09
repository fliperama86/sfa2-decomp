/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b6608_slot04_09[];
extern u8 data_801ba1b0_slot04_09[];
extern u32 data_801c7ae0_slot04_09[];

void func_801b0000_slot04_09(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u32 *dst;
    u8 *p;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    p = (u8 *)&obj->field_330;
    o->field_98 = data_801b6608_slot04_09;
    o->field_9c = data_801ba1b0_slot04_09;
    if (o->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c7ae0_slot04_09[i];
    }
    p[4] = 0;
    p[5] = 0;
    *(u32 *)p = 0;
    o->field_a0 = 0;
    o->field_a2 = 0;
    o->field_a3 = 0;
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 9;
        c->field_3c = o;
        *(u32 *)&o->field_2c = (u32)c;
        *(u32 *)&c->field_10 = *(u32 *)&o->field_10;
        *(u32 *)&c->field_14 = *(u32 *)&o->field_14;
        c->field_0b = o->field_0b;
        c->field_0e = o->field_0e;
        c->field_0c = o->field_0c;
        c->field_0d = o->field_0d;
        c->field_03 = o->kind;
        c->field_66 = o->side;
        c->field_7a = o->field_7a;
        c->field_7c = o->field_7c;
        c->field_90 = o->field_90;
        c->field_98 = o->field_98;
        c->field_9c = o->field_9c;
    }
}
