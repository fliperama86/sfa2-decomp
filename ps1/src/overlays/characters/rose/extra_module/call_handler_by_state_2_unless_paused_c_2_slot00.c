/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../../game.h"
#include "../../../../protos.h"
#include "../../../../externs.h"

extern ObjectFn data_800797f4_slot00[];
void select_box_tables_slot00_slot00(Object *obj);

void call_handler_by_state_2_unless_paused_c_2_slot00(Object *o) {
    Config *config = game_state.config;

    if (config->pause_flag_a == 0 && config->pause_flag_b == 0) {
        data_800797f4_slot00[o->state_2](o);
    }
    clear_or_set_flag_by_pos_x_limit(o);
}
