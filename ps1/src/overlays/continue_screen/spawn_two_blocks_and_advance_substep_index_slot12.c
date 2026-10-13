/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_8002d574_slot12;

void spawn_two_blocks_and_advance_substep_index_slot12(void) {
    Object *o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x7b;
        o->table_index = 0;
        o->spawn_x = 0;
        o->spawn_y = 0x1e1;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0x7b;
        o->table_index = 0x80;
        data_8002d574_slot12 = o;
    }
    screen_state->substep_index++;
}

void increment_substep_index_2_slot12(void) {
    screen_state->substep_index++;
}

void increment_substep_index_3_slot12(void) {
    screen_state->substep_index++;
}
