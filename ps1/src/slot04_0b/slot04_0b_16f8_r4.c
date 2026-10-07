/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1a18_slot04_0b(Object *obj) {
    Object *p;
    s16 y;

    func_80130efc(obj);
    if (*(u8 *)&obj->field_3a == 2) {
        obj->field_46 = 5;
        obj->field_07++;
        p = func_8011f0e8(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0xb;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->field_ac = obj->field_12a;
            p->field_ad = 0;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_26 = obj->field_26;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->field_7a = 0x60;
            p->field_af = 0;
            p->field_5c = 0;
            p->field_3c = obj;
            p->field_7c = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            obj->field_14c = (s32)p;
            obj->field_240++;
            func_80130efc(obj);
        }
    }
}
