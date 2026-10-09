/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Block172 *func_8011f1e0(void);

int func_80148ecc(Object *object, int n) {
    ref_other.p = (Object *) func_8011f1e0();
    if (ref_other.p != 0) {
        ref_other.p->field_00 = 1;
        ref_other.p->field_02 = 15;
        ref_other.p->field_03 = 0;
        ref_other.p->field_0e = object->field_0e;
        ref_other.p->field_1c = object->field_1c;
        ref_other.p->field_09 = 2;
        ref_other.p->field_48 = data_8017ceec[n & 0xff];
        ref_other.p->pos_x = object->pos_x;
        ref_other.p->pos_y = object->pos_y;
        ref_other.p->field_0b = object->field_0b;
        if (ref_other.p->field_0b != 0) {
            ref_other.p->pos_x = ref_other.p->pos_x - 16;
        } else {
            ref_other.p->pos_x = ref_other.p->pos_x + 16;
        }
        ref_other.p->pos_x = ref_other.p->pos_x - data_8017cecc[func_80151184() & 15];
        ref_other.p->pos_y = ref_other.p->pos_y - data_8017cecc[func_80151184() & 15];
        ref_other.p->field_7a = 0x60;
        ref_other.p->field_7c = 0x1e0;
        ref_other.p->field_90 = (void *)0x800fb100;
        ref_other.p->field_98 = &data_80172a48;
        ref_other.p->field_9c = &data_80173c9c;
        return 1;
    }
    return 0;
}
