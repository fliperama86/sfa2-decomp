/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_800797ec_slot00[];

void call_state_2_handler_unless_paused_slot00(Object *o) {
    call_state_2_handler_2684_unless_paused(o);
}

void call_handler_by_state_2_unless_paused_c_slot00(Object *o) {
    Config *config = game_state.config;

    if (config->pause_flag_a == 0 && config->pause_flag_b == 0) {
        data_800797ec_slot00[o->state_2](o);
    }
    clear_or_set_flag_by_pos_x_limit(o);
}
