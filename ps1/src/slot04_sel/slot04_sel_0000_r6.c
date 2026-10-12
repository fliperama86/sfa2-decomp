/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void save_player_select_values_slot04_sel(void) {
    Object *p;
    u8 k;
    p = &player_left;
    k = player_left.kind;
    if (game_state.field_1c != 0 || player_left.saved_side != k) {
        player_left.field_ef = 1;
    }
    if (p->mode_flag_2 == 0) {
        p->saved_side = k;
        p->select_value_2 = p->field_d8;
        p->select_value_1 = p->field_d4;
    }
    p = &player_right;
    k = player_right.kind;
    if (game_state.field_1c != 0 || player_right.saved_side != k) {
        player_right.field_ef = 1;
    }
    if (p->mode_flag_2 == 0) {
        p->saved_side = k;
        p->select_value_2 = p->field_d8;
        p->select_value_1 = p->field_d4;
    }
}
