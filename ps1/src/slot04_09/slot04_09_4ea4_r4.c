/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b536c_slot04_09(Object *obj) {
    Object *p;

    p = func_8011f0e8(obj);
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 9;
        p->field_03 = 0;
        p->field_66 = obj->field_66;
        p->field_65 = obj->field_65;
        p->field_49 = obj->field_49;
        p->field_ac = obj->field_12a;
        p->field_ae = obj->field_a0;
        p->field_ad = 0;
        p->field_0e = obj->field_0e;
        p->field_0b = obj->field_0b;
        p->field_0c = obj->field_0c;
        p->field_0d = obj->field_0d;
        p->field_26 = obj->field_26;
        p->field_5c = 0;
        *(u32 *)&p->field_a4 = 0x1000700;
        p->field_3c = obj;
        obj->field_14c = (s32)p;
        obj->field_240++;
        p->field_7a = 0x60;
        p->field_7c = 0x1e0;
        p->field_90 = obj->field_90;
        p->field_98 = obj->field_98;
        p->field_9c = obj->field_9c;
    }
}
