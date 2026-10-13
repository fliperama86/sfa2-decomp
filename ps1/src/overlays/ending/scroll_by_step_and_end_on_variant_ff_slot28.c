/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051df0_slot28;
extern u16 pad_1_held;
void update_cells_from_entry_table_slot28(Object *o);

void scroll_by_step_and_end_on_variant_ff_slot28(int *p) {
    u16 *a = &pad_1_held;
    u16 *b = &pad_2_held;
    Object *o = data_80051df0_slot28.p;
    p[1] += p[0];
    *(s32 *)&o->pos_y_frac -= p[0];
    if (((*a | *b) & 0x40) != 0) {
        p[1] += p[0];
        p[1] += p[0];
        p[1] += p[0];
        *(s32 *)&o->pos_y_frac -= p[0];
        *(s32 *)&o->pos_y_frac -= p[0];
        *(s32 *)&o->pos_y_frac -= p[0];
    }
    if (o->variant_index == 0xff) {
        if (((*a | *b) & 0x40) != 0) {
            p[1] -= p[0];
            p[1] -= p[0];
            p[1] -= p[0];
        } else {
            p[1] -= p[0];
        }
        screen_state->countdown_timer = 0xb4;
        screen_state->step_index_2++;
    }
    update_cells_from_entry_table_slot28((Object *)p);
    update_blocks();
    update_active_slab172_entries();
}
