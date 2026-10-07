/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801b6794_slot04_06[];
extern u8 data_801b9058_slot04_06[];
extern u32 data_801c5188_slot04_06[];

Object *func_8011f32c(void);

void func_801b0000_slot04_06(Object *obj) {
    u32 *dst;
    int i;
    Object *c;
    u8 s;

    dst = (u32 *)0x1f800100;
    obj->field_98 = data_801b6794_slot04_06;
    obj->field_9c = data_801b9058_slot04_06;
    if (obj->side == 0) {
        dst = (u32 *)0x1f800050;
    }
    i = 0;
    for (; i < 0x28; i++) {
        dst[i] = data_801c5188_slot04_06[i];
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 6;
        c->field_03 = 0;
        s = obj->field_0b;
        obj->field_28 = (u32)c;
        c->field_3c = obj;
        c->field_0b = s;
        c->field_66 = obj->side;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
}
