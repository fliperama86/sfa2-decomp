/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007a12c_slot00[];

void func_80078e44_slot00(Object *o) {
    Object *obj = o;
    Object *p = (Object *)func_8011f1e0();
    Object *src;
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 0xc;
        p->field_08 = 0x20;
        p->field_03 = 0xa;
        p->field_0e = obj->field_0e;
        src = obj->field_3c;
        p->field_0c = src->field_0c;
        p->field_0d = src->field_0d;
        p->field_26 = obj->field_26;
        p->field_3c = o;
        p->field_0b = obj->field_0b;
        p->pos_x = (u16)o->pos_x;
        p->pos_y = (u16)o->pos_y;
        if (player_left.kind == 9) {
            src = &player_left;
        } else {
            src = &player_left + 1;
        }
        p->field_90 = src->field_90;
        p->field_98 = src->field_98;
        p->field_9c = src->field_9c;
        p->field_7a = src->field_7a;
        p->field_7c = src->field_7c;
        p->field_66 = src->side;
    }
}

void func_80078f50_slot00(Object *obj) {
    data_8007a12c_slot00[obj->field_04](obj);
}
