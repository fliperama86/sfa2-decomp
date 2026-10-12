/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void set_phase_two_clear_step_slot0f(Object *obj);

void spawn_screen_handler_blocks_slot0f(Object *obj) {
    Block172 *b;
    u8 *flag;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    flag = &data_801903c5;
    b->handler_index = 0x90;
    b->block_mode = *flag;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 0;
    b->handler_index = 0x96;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 1;
    b->handler_index = 0x96;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 2;
    b->handler_index = 0x96;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 3;
    b->handler_index = 0x96;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 4;
    b->handler_index = 0x96;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 5;
    b->handler_index = 0x96;
    if (*flag != 0) return;
    b = pop_block_from_pool();
    if (b == 0) goto fail;
    b->active_flag = 1;
    b->block_mode = 0;
    b->handler_index = 0x98;
    if (obj->field_01 != 0) return;
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
    return;
fail:
    set_phase_two_clear_step_slot0f(obj);
}
