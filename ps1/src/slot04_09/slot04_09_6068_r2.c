/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Object *func_8011f32c(void);

void func_801b6210_slot04_09(Object *obj) {
    Object *c = func_8011f32c();
    if (c != 0) {
        c->field_00 = 1;
        c->field_02 = 0x1b;
        c->field_03 = 0;
        c->field_0e = obj->field_0e;
        c->field_0c = obj->field_0c;
        c->field_0d = obj->field_0d;
        c->field_26 = obj->field_26;
        c->field_48 = 0x31;
        c->field_0b = obj->field_0b ^ 1;
        c->field_3c = obj;
        ((Slot04aObj *)obj)->field_30 = (u32)c;
        if (obj->field_0b != 0) {
            c->field_4c = 0xa0000;
            c->field_54 = 0xffff6000;
        } else {
            c->field_4c = 0xfff60000;
            c->field_54 = 0xa000;
        }
        c->field_66 = obj->side;
        c->field_7a = obj->field_7a;
        c->field_7c = obj->field_7c;
        c->field_90 = obj->field_90;
        c->field_98 = obj->field_98;
        c->field_9c = obj->field_9c;
    }
}
