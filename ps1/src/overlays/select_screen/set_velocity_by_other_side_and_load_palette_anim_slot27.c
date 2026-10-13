/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void load_palette_anim_from_table_index_slot27(Object *obj);

void set_velocity_by_other_side_and_load_palette_anim_slot27(Object *obj) {
    int *x = (int *)scratch_word_a;
    int *y = scratch_word_b;
    obj->state_3 = 0;
    obj->state_2 = obj->state_2 + 1;
    *x = -0x10;
    *y = -0x90;
    if (ref_other.p->side != 0) {
        *x = 0x10;
        *y = 0x90;
    }
    obj->vel_x = *x;
    obj->vel_y = *y;
    *x = 4;
    *x = ref_other.p->field_d8 + 4;
    load_palette_anim_from_table_index_slot27(obj);
}
