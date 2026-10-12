/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_object_unless_paused_check_x_margin_slot04_0b(Object *o) {
    Config *config = game_state.config;

    if ((config->pause_flag_b | config->pause_flag_a) == 0) {
        *(s32 *)&o->pos_x_frac += o->vel_x;
        *(s32 *)&o->pos_y_frac += o->vel_y;
        advance_sequence_checked(o);
        if (o->vel_table_index >= 0xc) {
            set_state_0_if_x_outside_margin_wide(o);
        } else {
            set_state_0_if_x_outside_margin_narrow(o);
        }
    }
    clear_or_set_flag_by_pos_x_limit(o);
}
