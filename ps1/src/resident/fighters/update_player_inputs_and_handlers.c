/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

extern u16 pad_1_held;

void update_player_inputs_and_handlers(void) {
    Object *p;
    player_left.prev_input_bits = player_left.input_bits;
    player_right.prev_input_bits = player_right.input_bits;
    if (game_state.versus_flag == 0 || input_block[0] == 0) {
        player_left.input_bits = pad_1_held;
        player_right.input_bits = pad_2_held;
    }
    if (game_state.field_31 == 0) {
        p = &player_left;
        count_down_cursor_timer_a_and_set_cursor(p);
        call_handler_by_side(p, table_80185cf4);
        p++;
        count_down_cursor_timer_a_and_set_cursor(p);
        call_handler_by_side(p, table_80185e50);
    }
}
