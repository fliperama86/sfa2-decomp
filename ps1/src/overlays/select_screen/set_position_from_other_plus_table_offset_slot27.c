/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern s16 data_80028128_slot27[];

void set_position_from_other_plus_table_offset_slot27(Object *obj) {
    int *x = scratch_word_a;
    int *y = scratch_word_b;
    *y = (*(Config **)&ref_aux)->table_index * 4;
    *y = (obj->table_index + *y) * 2;
    *x = data_80028128_slot27[scratch_word_b[0]];
    *y = data_80028128_slot27[scratch_word_b[0] + 1];
    *x = ref_other.p->pos_x + *x;
    *y = ref_other.p->pos_y + *y;
    *x += obj->vel_x;
    *y += obj->vel_y;
    obj->pos_x = *x;
    obj->pos_y = *y;
}
