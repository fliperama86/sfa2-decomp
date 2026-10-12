/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80017bf0_slot27[])(void);
extern u8 data_80029324_slot27;
void spawn_five_pool_objects_10_indexed_slot27(void);

void call_handler_by_hud_step_index_2_2_slot27(void) {
    data_80017bf0_slot27[data_8018f5a0->step_index_2]();
}

void advance_step_and_reset_selection_b0_set_slot27(void) {
    HudState *h = data_8018f5a0;
    h->step_index_2++;
    h->pass_index = 0;
    h->field_54 = 0;
    game_state.b0_flag = 1;
    game_state.countdown_2 = 0;
    game_state.timer_or_bits = 0;
    game_state.field_80 = 0;
    game_state.selection_value = 0xff;
    game_state.b2_count = 0;
}

void advance_step_index_2_and_spawn_block_objects_slot27(void) {
    HudState *h = data_8018f5a0;
    Block172 *b;
    data_80029324_slot27 = 0xff;
    h->step_index_2++;
    b = pop_block_from_pool();
    if (b != 0) {
        b->active_flag = 1;
        b->handler_index = 0xa8;
        b->block_mode = 0;
    }
    b = pop_block_from_pool();
    if (b != 0) {
        b->active_flag = 1;
        b->handler_index = 0x10;
        b->block_mode = 7;
    }
    spawn_five_pool_objects_10_indexed_slot27();
    game_state.timer_or_bits = 8;
}
