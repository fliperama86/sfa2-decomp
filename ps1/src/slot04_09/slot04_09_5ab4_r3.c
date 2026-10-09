/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5d3c_slot04_09(Object *obj) {
    Object *a;
    Object *b;
    Object *c;
    Object *t;
    u8 side;
    int three;
    if (data_801a6960 >= 3) {
        side = obj->side;
        three = 3;
        obj->field_246 = three;
        c = func_8011f0e8(obj);
        if (c != 0) {
            a = c;
        }
        c = func_8011f0e8(obj);
        if (c != 0) {
            b = c;
        }
        c = func_8011f0e8(obj);
        if (c != 0) {
            if (b < a) {
                t = a;
                a = b;
                b = t;
            }
            if (c < a) {
                t = a;
                a = c;
                c = t;
            }
            if (c < b) {
                t = b;
                b = c;
                c = t;
            }
            c->field_00 = 1;
            c->field_02 = 0x17;
            c->field_03 = 1;
            c->field_ad = 1;
            c->field_ac = obj->field_12a;
            c->field_0e = obj->field_0e;
            c->field_48 = side;
            c->field_66 = obj->field_66;
            c->field_65 = obj->field_65;
            c->field_49 = obj->field_49;
            c->field_4b = obj->field_4b;
            c->field_4c = side;
            c->field_46 = 8;
            c->field_20 = 8;
            c->field_3c = obj;
            c->field_0c = obj->field_0c;
            c->field_0d = obj->field_0d + 3;
            *(s32 *)&c->field_a4 = 0x10;
            c->field_7a = obj->field_7a;
            c->field_7c = obj->field_7c;
            c->field_90 = obj->field_90;
            c->field_98 = obj->field_98;
            c->field_9c = obj->field_9c;
            b->field_00 = 1;
            b->field_02 = 0x17;
            b->field_03 = 2;
            b->field_ad = 1;
            b->field_ac = obj->field_12a;
            b->field_0e = obj->field_0e;
            b->field_48 = side;
            b->field_66 = obj->field_66;
            b->field_65 = obj->field_65;
            b->field_49 = obj->field_49;
            b->field_4b = obj->field_4b;
            b->field_4c = side;
            b->field_46 = 0x10;
            b->field_20 = 0x10;
            b->field_3c = obj;
            b->field_0c = obj->field_0c;
            b->field_0d = obj->field_0d + 4;
            *(s32 *)&b->field_a4 = 8;
            b->field_7a = obj->field_7a;
            b->field_7c = obj->field_7c;
            b->field_90 = obj->field_90;
            b->field_98 = obj->field_98;
            b->field_9c = obj->field_9c;
            a->field_00 = 1;
            a->field_02 = 0x17;
            a->field_03 = three;
            a->field_ad = 1;
            a->field_ac = obj->field_12a;
            a->field_0e = obj->field_0e;
            a->field_48 = side;
            a->field_66 = obj->field_66;
            a->field_65 = obj->field_65;
            a->field_49 = obj->field_49;
            a->field_4b = obj->field_4b;
            a->field_4c = side;
            a->field_46 = 0x18;
            a->field_20 = 0x18;
            a->field_3c = obj;
            a->field_0c = obj->field_0c;
            a->field_0d = obj->field_0d + 4;
            *(s32 *)&a->field_a4 = 0;
            a->field_7a = obj->field_7a;
            a->field_7c = obj->field_7c;
            a->field_90 = obj->field_90;
            a->field_98 = obj->field_98;
            a->field_9c = obj->field_9c;
        }
    }
}
