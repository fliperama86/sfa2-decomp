/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Object *func_8011f32c(void);

void func_801b6068_slot04_09(Object *obj) {
    u8 f;
    Object *c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x1b;
        c->field_03 = 0;
        c->field_0e = obj->field_0e;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_26 = obj->field_26;
        f = obj->field_0b;
        c->field_48 = 0x30;
        c->field_3c = obj;
        c->field_0b = f;
        ((Slot04aObj *)obj)->field_30 = (u32)c;
        c->field_4c = 0x50000;
        c->field_54 = -0x5000;
        c->field_66 = obj->side;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
    c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x1b;
        c->field_03 = 1;
        c->field_0e = obj->field_0e;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_26 = obj->field_26;
        f = obj->field_0b;
        c->field_48 = 0x30;
        c->field_3c = obj;
        c->field_0b = f;
        ((Slot04aObj *)obj)->field_34 = (u32)c;
        c->field_4c = 0xfffb0000;
        c->field_54 = 0x5000;
        c->field_66 = obj->side;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
}
