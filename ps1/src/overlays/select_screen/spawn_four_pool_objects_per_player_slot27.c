/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void spawn_four_pool_objects_per_player_slot27(void) {
    int i;
    int one;
    i = 0;
    one = 1;
    for (; i < 2; i++) {
        Object *p = &player_left + i;
        Object *o;
        if ((game_state.side_mask >> p->side) & 1) {
            p->field_d4 = 0;
        }
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            o->active_count = one;
            o->field_02 = 0xac;
            o->other = p;
        }
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            o->active_count = one;
            o->field_02 = 0xc;
            o->other = p;
        }
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            o->field_02 = 0x10;
            o->active_count = one;
            o->table_index = 3;
            o->other = p;
        }
        o = (Object *)pop_block_from_pool();
        if (o != 0) {
            o->active_count = one;
            o->field_02 = 0xd;
            o->other = p;
        }
    }
}
