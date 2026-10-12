/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c45a8_slot04_04[];

void call_state_1_table_handler_then_cull_check_2_slot04_04(Object *o) {
    data_801c45a8_slot04_04[o->state_1](o);
    clear_or_set_flag_by_pos_x_limit(o);
}
