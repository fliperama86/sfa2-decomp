/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int spawn_object_with_position_table(u8 a, u8 b) {
    u16 tbl[3][2] = { { 458, 37 }, { 0xffb6, 37 }, { 458, 112 } };
    u16 (*t)[2] = tbl;
    ref_other.p = (Object *) pop_block_from_pool();
    if (ref_other.p == 0) return 0;
    ref_other.p->active_count = 1;
    ref_other.p->field_02 = 0x27;
    ref_other.p->base_x = 0x340;
    ref_other.p->base_y = 0;
    ref_other.p->spawn_x = 0xb0;
    ref_other.p->spawn_y = 0x1e0;
    ref_other.p->image_row = 0xd;
    ref_other.p->field_0c = 1;
    ref_other.p->ot_depth = 0;
    ref_other.p->table_index = a;
    ref_other.p->variant_index_2 = b;
    ref_other.p->step_count = 0;
    ref_other.p->field_a3 = 0;
    ref_other.p->field_a4 = 0;
    if (a == 2) {
        ref_other.p->table_index = 0;
        ref_other.p->image_row = 0;
        ref_other.p->field_a4 = 1;
    }
    ref_other.p->pos_x = t[ref_other.p->table_index][0];
    ref_other.p->pos_y = t[ref_other.p->table_index][1];
    ref_other.p->field_01 = 0;
    if (a == 2) {
        ref_other.p->table_index = 1;
        ref_other.p->pos_y = 0x70;
        ref_other.p->health = 0;
    }
    if (ref_other.p->variant_index_2 == 1) {
        ref_other.p->image_row = 0;
        ref_other.p->pos_y = 0x70;
    }
    if (ref_other.p->table_index >= 9) {
        ref_other.p->spawn_x = 0x80;
        ref_other.p->image_row = 0;
    }
}
