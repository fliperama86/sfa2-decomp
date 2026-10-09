/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Object *func_8011f0e8(Object *unused);

void func_801b2d20_slot04_04(Object *obj) {
    Object *p;

    if (((s16)obj->field_3a & 0x8000) == 0) {
        func_80130efc(obj);
    } else {
        obj->field_07++;
        p = func_8011f0e8(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0x15;
            p->field_ad = 1;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_26 = obj->field_26;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->pos_x = obj->pos_x;
            p->pos_y = obj->pos_y;
            p->field_70 = obj->field_70;
            p->field_3c = obj;
            p->field_7a = obj->field_7a;
            p->field_7c = obj->field_7c;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            func_801204f4(obj, obj->side, 0x11);
        }
        func_801307e0(obj, obj->field_12a + 0x5a);
    }
}
