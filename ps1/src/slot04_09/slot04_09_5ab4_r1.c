/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5ab4_slot04_09(Object *o) {
    Object *obj = o;
    Object *p = (Object *)func_8011f1e0();
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 0xc;
        p->field_08 = 0x20;
        p->field_03 = 0x14;
        p->field_0e = obj->field_0e;
        p->field_0c = obj->field_0c;
        p->field_0d = obj->field_0d;
        p->field_26 = obj->field_26;
        p->field_3c = o;
        p->pos_x = (u16)o->pos_x;
        p->pos_y = (u16)o->pos_y;
        p->field_0b = obj->field_0b;
        p->field_90 = obj->field_90;
        p->field_98 = obj->field_98;
        p->field_9c = obj->field_9c;
        p->field_7a = obj->field_7a;
        p->field_7c = obj->field_7c;
        p->field_66 = obj->side;
    }
}
