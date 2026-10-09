/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b52b4_slot04_07[];
extern u8 data_801b7630_slot04_07[];
extern u32 data_801c1d98_slot04_07[];

Object *func_8011f32c(void);

void func_801b0000_slot04_07(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b52b4_slot04_07;
    obj->field_9c = data_801b7630_slot04_07;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c1d98_slot04_07[i];
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 7;
        c->field_3c = obj;
        obj->field_28 = (u32)c;
        c->field_03 = obj->kind;
        c->field_66 = obj->side;
        c->field_0d = obj->field_0d;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
    if (game_state.field_42 == 0) {
        if (obj->side == 0) {
            obj->pos_x = 0x1a0;
        } else {
            obj->pos_x = 0x380;
        }
        obj->pos_y = obj->pos_y - 0x38;
    }
}
