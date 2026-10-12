/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_80148ecc(Object *object, int n) {
    ref_other.p = (Object *) func_8011f1e0();
    if (ref_other.p != 0) {
        ref_other.p->active_count = 1;
        ref_other.p->field_02 = 15;
        ref_other.p->table_index = 0;
        ref_other.p->field_0e = object->field_0e;
        ref_other.p->field_1c = object->field_1c;
        ref_other.p->ot_depth = 2;
        ref_other.p->variant_index = data_8017ceec[n & 0xff];
        ref_other.p->pos_x = object->pos_x;
        ref_other.p->pos_y = object->pos_y;
        ref_other.p->facing_side = object->facing_side;
        if (ref_other.p->facing_side != 0) {
            ref_other.p->pos_x = ref_other.p->pos_x - 16;
        } else {
            ref_other.p->pos_x = ref_other.p->pos_x + 16;
        }
        ref_other.p->pos_x = ref_other.p->pos_x - data_8017cecc[get_random_byte() & 15];
        ref_other.p->pos_y = ref_other.p->pos_y - data_8017cecc[get_random_byte() & 15];
        ref_other.p->spawn_x = 0x60;
        ref_other.p->spawn_y = 0x1e0;
        ref_other.p->field_90 = (void *)0x800fb100;
        ref_other.p->data_ptr_a = &data_80172a48;
        ref_other.p->data_ptr_b = &data_80173c9c;
        return 1;
    }
    return 0;
}
