/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_phase_two_clear_step_slot12(Object *obj);

void spawn_pool_blocks_or_fail_slot12(Object *obj) {
    Block172 *b;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->handler_index = 0xa0;
    b->block_mode = game_state.mode_flag_2;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 0;
    b->handler_index = 0xa1;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 1;
    b->handler_index = 0xa1;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 2;
    b->handler_index = 0xa1;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 3;
    b->handler_index = 0xa1;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 4;
    b->handler_index = 0xa1;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 5;
    b->handler_index = 0xa1;
    if (game_state.mode_flag_2 == 0) {
        b = pop_block_from_pool();
        if (b == 0) goto fail;
        b->active_flag = 1;
        b->block_mode = 0;
        b->handler_index = 0xa3;
        b = pop_block_from_pool();
        if (b == 0) goto fail;
        b->active_flag = 1;
        b->block_mode = 0;
        b->handler_index = 0x92;
        b = pop_block_from_pool();
        if (b == 0) goto fail;
        b->active_flag = 1;
        b->block_mode = 1;
        b->handler_index = 0x92;
    }
    return;
fail:
    set_phase_two_clear_step_slot12(obj);
}
