/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot06_00Rec3010 data_801f3010_slot06_00[];

void func_801e8f74_slot06_00(Object *obj) {
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
    func_8011ffdc(obj);
}

void func_801e9060_slot06_00(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
