/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b5ed8_slot04_06(Object *obj) {
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
        ref_other.p->field_00 = 1;
        ref_other.p->field_02 = 0x16;
        ref_other.p->field_03 = 0;
        ref_other.p->field_0e = obj->field_0e;
        ref_other.p->field_1c = obj->field_1c;
        ref_other.p->field_09 = obj->field_09;
        ref_other.p->pos_x = obj->pos_x;
        ref_other.p->pos_y = obj->pos_y;
        ref_other.p->field_0b = obj->field_0b;
        ref_other.p->field_7a = obj->field_7a;
        ref_other.p->field_7c = obj->field_7c;
        ref_other.p->field_02 = 0x10;
        ref_other.p->field_08 = 0x20;
        ref_other.p->field_90 = (void *)0x800fb100;
        ref_other.p->field_66 = obj->field_66;
        ref_other.p->field_98 = data_80172a48;
        ref_other.p->field_9c = data_80173c9c;
        return 1;
    }
    return 0;
}
