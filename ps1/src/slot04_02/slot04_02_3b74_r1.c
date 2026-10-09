/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The call of func_8011f0e8 passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 3 instruction slots. */
void func_801b3b74_slot04_02(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    Object *p;
    u16 y;

    func_80130efc(o);
    if (obj->field_3a != 0) {
        o->field_07++;
        o->field_46 = obj->field_46 | 0x600;
        p = ((Object *(*)(void))func_8011f0e8)();
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 2;
            p->field_03 = 4;
            p->field_66 = o->field_66;
            p->field_65 = o->field_65;
            p->field_ac = (o->field_12a >> 1) + 6;
            p->field_5c = o->field_12a >> 1;
            p->field_ad = 0;
            p->field_0b = o->field_0b;
            p->field_0e = o->field_0e;
            p->field_0c = o->field_0c;
            p->field_0d = o->field_0d;
            p->field_26 = o->field_26;
            p->pos_x = o->pos_x;
            p->pos_y = o->pos_y;
            y = o->field_70;
            p->field_7a = 0x60;
            p->field_3c = o;
            p->field_7c = 0x1e0;
            p->field_70 = y;
            p->field_90 = o->field_90;
            p->field_98 = o->field_98;
            p->field_9c = o->field_9c;
            o->field_14c = (s32)p;
            o->field_240++;
            func_801204f4(o, o->side, 0xb);
        }
    }
}
