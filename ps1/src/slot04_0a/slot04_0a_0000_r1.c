/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b5174_slot04_0a[];
extern u8 data_801b6e48_slot04_0a[];
extern u32 data_801c0450_slot04_0a[];

void func_801b0000_slot04_0a(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b5174_slot04_0a;
    obj->field_9c = data_801b6e48_slot04_0a;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c0450_slot04_0a[i];
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0xa;
        c->field_03 = obj->kind;
        c->field_3c = obj;
        obj->field_28 = (u32)c;
        *(u32 *)&c->field_10 = *(u32 *)&obj->field_10;
        *(u32 *)&c->field_14 = *(u32 *)&obj->field_14;
        c->field_48 = 0xff;
        c->field_0b = obj->field_0b;
        c->field_0e = obj->field_0e;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_66 = obj->side;
        c->field_0d = obj->field_0d;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
    if (game_state.field_42 == 0) {
        obj->field_45 = 1;
        obj->pos_y = obj->pos_y - 0x30;
    }
}
