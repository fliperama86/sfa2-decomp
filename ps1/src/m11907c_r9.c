/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966;

void update_player_inputs_and_handlers(void) {
    Object *p;
    player_left.prev_input_bits = player_left.input_bits;
    player_right.prev_input_bits = player_right.input_bits;
    if (game_state.versus_flag == 0 || data_801a6984[0] == 0) {
        player_left.input_bits = data_801a6966;
        player_right.input_bits = data_801a6972;
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
