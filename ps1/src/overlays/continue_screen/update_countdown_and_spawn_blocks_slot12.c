/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_8002d568_slot12;
extern Object *data_8002d56c_slot12;

void update_countdown_and_spawn_blocks_slot12(void) {
    u16 *p = &game_state.countdown_2;
    int t = *p - 1;
    *p = t;
    if ((s16)t < 0) {
        Block172 *b;
        data_8018f5a0->step_index = data_8018f5a0->step_index + 1;
        *p = 0x2f;
        if (game_state.field_44 & 0x80) {
            play_side_sound_at(data_8002d56c_slot12, game_state.sound_side ^ 1, 5);
        } else {
            play_side_sound_at(data_8002d568_slot12, data_8002d568_slot12->side, 5);
        }
        b = pop_block_from_pool();
        if (b != 0) {
            b->active_flag = 1;
            b->handler_index = 0x26;
            b->block_mode = 1;
        }
        b = pop_block_from_pool();
        if (b != 0) {
            b->active_flag = 1;
            b->handler_index = 0x7e;
            b->block_mode = 0;
        }
        b = pop_block_from_pool();
        if (b != 0) {
            b->active_flag = 1;
            b->handler_index = 0x7e;
            b->block_mode = 1;
        }
    }
}
