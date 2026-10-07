/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The call of func_8011f0e8 passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 5 instruction slots. */
void func_801b2b2c_slot04_02(Object *obj) {
    Object *p;
    s16 t;
    u16 y;

    func_80130efc(obj);
    t = obj->field_3a;
    if (!(t & 0x8000)) {
        if ((t & 0xff) != 0) {
            obj->field_3a = t & 0xff00;
            p = ((Object *(*)(void))func_8011f0e8)();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 2;
                p->field_03 = 6;
                p->field_66 = obj->field_66;
                p->field_65 = obj->field_65;
                p->field_4b = obj->field_4b;
                p->field_ac = (obj->field_12a >> 1) + 9;
                p->field_5c = obj->field_12a + 3;
                p->field_ad = 1;
                p->field_0b = obj->field_0b;
                p->field_0e = obj->field_0e;
                p->field_0c = obj->field_0c;
                p->field_0d = obj->field_0d;
                p->field_26 = obj->field_26;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                y = obj->field_70;
                p->field_7a = 0x60;
                p->field_3c = obj;
                p->field_7c = 0x1e0;
                p->field_70 = y;
                p->field_90 = obj->field_90;
                p->field_98 = obj->field_98;
                p->field_9c = obj->field_9c;
                obj->field_14c = (s32)p;
                obj->field_240++;
                func_801204f4(obj, obj->side, 0xb);
            }
        }
    } else {
        func_801312b8(obj);
    }
}
