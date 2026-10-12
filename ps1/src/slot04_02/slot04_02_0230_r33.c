/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7800_slot04_02[];

void call_state_2_handler_unless_paused_slot04_02(Object *o) {
    call_state_2_handler_2684_unless_paused(o);
}

void call_handler_by_state_2_3_slot04_02(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c7800_slot04_02[o->state_2](o);
    }
    clear_or_set_flag_by_pos_x_limit(o);
}
