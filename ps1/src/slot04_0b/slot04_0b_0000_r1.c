/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b4e5c_slot04_0b[];
extern u8 data_801b7274_slot04_0b[];
extern u32 data_801c23e0_slot04_0b[];

Object *func_8011f32c(void);
Block172 *func_8011f1e0(void);
void func_80130678(Object *object, int index);
void func_801b05c4_slot04_0b(Object *obj);

void func_801b0000_slot04_0b(Object *obj) {
    u32 *dst;
    u32 i;
    Object *c;
    Object *d;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b4e5c_slot04_0b;
    obj->field_9c = data_801b7274_slot04_0b;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c23e0_slot04_0b[i];
    }
    d = func_8011f32c();
    if (d != 0) {
        d->field_00 = 1;
        d->field_02 = 0xb;
        d->field_03 = obj->kind;
        d->field_3c = obj;
        d->field_0c = obj->field_0c;
        d->field_0e = obj->field_0e;
        obj->field_28 = (u32)d;
        d->field_66 = obj->side;
        d->field_0d = obj->field_0d;
        d->field_7a = obj->field_7a;
        d->field_7c = obj->field_7c;
        d->field_90 = obj->field_90;
        d->field_98 = obj->field_98;
        d->field_9c = obj->field_9c;
    }
    if (game_state.field_42 == 0) {
        if (obj->other->kind == 0) {
            if (obj->field_cd != 0) {
                obj->field_06 = 1;
                if (obj->side == 0) {
                    obj->pos_x = obj->pos_x + 0x50;
                } else {
                    obj->pos_x = obj->pos_x - 0x50;
                }
                func_801b05c4_slot04_0b(obj);
                c = (Object *)func_8011f1e0();
                if (c != 0) {
                    c->field_00 = 1;
                    c->field_02 = 6;
                    c->field_03 = 0;
                    c->field_3c = obj;
                    c->field_08 = 0x20;
                    c->field_90 = obj->field_90;
                    c->field_7a = obj->field_7a;
                    c->field_7c = obj->field_7c;
                    c->field_66 = obj->side;
                    c->field_98 = obj->field_98;
                    c->field_9c = obj->field_9c;
                }
                c = (Object *)func_8011f1e0();
                if (c != 0) {
                    c->field_00 = 1;
                    c->field_02 = 6;
                    c->field_03 = 1;
                    c->field_3c = obj;
                    c->field_08 = 0x20;
                    c->field_66 = obj->side;
                    c->field_90 = obj->field_90;
                    c->field_98 = obj->field_98;
                    c->field_9c = obj->field_9c;
                    if (c->field_66 == 0) {
                        c->field_7a = 0x110;
                    } else {
                        c->field_7a = 0x130;
                    }
                    c->field_7c = 0x1e0;
                }
                func_80130678(obj, 0x30);
            } else {
                obj->field_06 = 2;
                func_80130678(obj, 0x32);
            }
        } else {
            obj->field_06 = 0;
        }
    } else {
        obj->field_06 = 0;
    }
}
