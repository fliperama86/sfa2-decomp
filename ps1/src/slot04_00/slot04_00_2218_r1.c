/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2218_slot04_00(Object *obj) {
    Object *p;
    s16 t;
    s16 y;

    func_80130efc(obj);
    t = obj->field_3a;
    if (t & 0x8000) {
        func_801312b8(obj);
    } else if ((t & 0xff00) == 0 && (t & 0xff) != 0) {
        obj->field_3a = 0;
        p = func_8011f0e8(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 0;
            p->field_03 = 2;
            p->field_65 = obj->field_65;
            p->field_66 = obj->field_66;
            p->field_4b = obj->field_4b;
            p->field_ac = obj->field_12a + 6;
            p->field_5c = (obj->field_12a >> 1) + 2;
            p->field_ad = 1;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_26 = obj->field_26;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->field_7a = 0x60;
            p->field_3c = obj;
            p->field_7c = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            obj->field_14c = (s32)p;
            obj->field_240++;
            func_801204f4(obj, obj->side, 0xb);
        }
    }
}
