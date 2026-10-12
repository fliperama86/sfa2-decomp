/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c56c4_slot04_06[];

void call_state_1_table_handler_then_cull_check_slot04_06(Object *o) {
    data_801c56c4_slot04_06[o->state_1](o);
    clear_or_set_flag_by_pos_x_limit(o);
}
