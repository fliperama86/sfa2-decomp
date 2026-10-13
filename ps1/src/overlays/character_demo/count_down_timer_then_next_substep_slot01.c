/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_8002ceb0_slot01;
extern Object *data_80055f40_slot01;

void count_down_timer_then_next_substep_slot01(void) {
    int t = game_state.timer_or_bits - 1;
    game_state.timer_or_bits = t;
    if ((s16)t < 0 || is_pad_pressed_for_side_bits_or_mode() != 0) {
        screen_state->substep_index++;
        game_state.status_flags = 0xff;
        if (game_state.mode != 0) {
            game_state.field_09 = 0;
        }
    }
}

void start_timer_and_spawn_hud_object_slot01(void) {
    Object *s0;
    Object *p;

    s0 = game_state.focus_object;
    screen_state->substep_index++;
    game_state.timer_or_bits = 0xb4;
    data_8002ceb0_slot01->spawn_y = 0x1e9;
    p = (Object *)pop_block_from_pool();
    if (p) {
        p->active_count = 1;
        p->field_02 = 0x1b;
        p->table_index = 0x80;
        p->spawn_x = 0x70;
        p->parent = s0;
        p->spawn_y = 0x1ea;
        data_80055f40_slot01 = p;
    }
}
