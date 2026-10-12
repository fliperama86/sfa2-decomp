/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06_00Rec3010 data_801f3010_slot06_00[];

void update_alternating_scroll_object_slot06_00(Object *obj) {
    Slot06_00Rec3010 *r = &data_801f3010_slot06_00[obj->table_index];
    u32 m = 0x3ffffff;

    if ((game_state.field_65 | game_state.field_74) == 0) {
        r->pos_x_accum = (r->pos_x_accum + r->step_x_a) & m;
        r->alt_pos_x_accum = (r->alt_pos_x_accum + r->step_x_b) & m;
    }
    if (((game_state.tick_count + game_state.field_24) & 1) != 0) {
        *(u32 *)&obj->pos_x_frac = r->alt_pos_x_accum;
        *(u32 *)&obj->pos_y_frac = r->pos_y_b;
        obj->sequence = r->sequence_next;
    } else {
        *(u32 *)&obj->pos_x_frac = r->pos_x_accum;
        *(u32 *)&obj->pos_y_frac = r->pos_y_a;
        obj->sequence = r->sequence_start;
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void release_object_record_slot06_00(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}
