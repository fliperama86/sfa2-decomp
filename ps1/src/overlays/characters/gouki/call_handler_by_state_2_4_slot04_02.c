/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c7818_slot04_02[];

void call_handler_by_state_2_4_slot04_02(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c7818_slot04_02[o->state_2](o);
    }
    clear_or_set_flag_by_pos_x_limit(o);
}
