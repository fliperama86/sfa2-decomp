/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800175b4_slot12[])(void);
void reset_hud_and_game_state_to_phase_4_slot12(void);

void spawn_block_with_saved_side_slot12(void) {
    Block172 *b = pop_block_from_pool();
    if (b != 0) {
        Object *p;
        b->active_flag = 1;
        b->handler_index = 0x7c;
        p = &player_left;
        if ((game_state.mode_copy & 1) == 0) {
            p = p + 1;
        }
        b->block_mode = p->saved_side;
    }
}

void reset_hud_to_phase_4_and_init_pools_slot12(void) {
    reset_hud_and_game_state_to_phase_4_slot12();
    game_state.field_44 = 0xff;
    clear_arrays_and_init_pools_and_sprite_tables();
}

void call_step_table_then_update_blocks_and_text_slot12(void) {
    data_800175b4_slot12[screen_state->step_index]();
    update_blocks();
    update_active_slab172_entries();
    queue_player_status_text_items();
}
