/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800795f8_slot2b(Object *o) {
    Object *p = o->field_3c;
    Object *n = (Object *)func_8011f1e0();
    if (n != 0) {
        n->field_00 = 1;
        n->field_02 = 0x3b;
        n->field_03 = 1;
        n->field_02 = 0x14;
        n->field_09 = 0;
        n->pos_x = (u16)o->pos_x;
        n->pos_y = (u16)o->pos_y - 0x10;
        n->field_48 = 0x51;
        n->field_65 = o->field_65;
        n->field_7a = 0x60;
        n->field_3c = p;
        n->field_44 = 1;
        n->field_7c = 0x1e0;
        n->field_0d = o->field_0d;
        n->field_08 = 0x20;
        n->field_90 = o->field_90;
        n->field_66 = o->field_66;
        n->field_98 = o->field_98;
        n->field_9c = o->field_9c;
        func_801204f4(o, p->side, 0xf);
    }
}
