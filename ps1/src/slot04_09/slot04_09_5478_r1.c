/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5478_slot04_09(Object *obj) {
    Object *n = func_8011f0e8(obj);

    if (n != 0) {
        n->field_00 = 1;
        n->field_02 = 9;
        n->field_03 = 0;
        n->field_66 = obj->field_66;
        n->field_65 = obj->field_65;
        n->field_49 = obj->field_49;
        n->field_ac = obj->field_12a;
        n->field_ae = 0;
        n->field_ad = 1;
        n->field_4b = obj->field_4b;
        n->field_0e = obj->field_0e;
        n->field_0b = obj->field_0b;
        n->field_0c = obj->field_0c;
        n->field_0d = obj->field_0d;
        n->field_26 = obj->field_26;
        n->field_5c = 2;
        *(u32 *)&n->field_a4 = 0x1000800;
        n->field_3c = obj;
        obj->field_14c = (s32)n;
        obj->field_240++;
        n->field_7a = 0x60;
        n->field_7c = 0x1e0;
        n->field_90 = obj->field_90;
        n->field_98 = obj->field_98;
        n->field_9c = obj->field_9c;
    }
}
