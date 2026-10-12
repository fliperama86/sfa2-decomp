/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c056c_slot04_00[];

void call_state_2_handler_unless_paused_slot04_00(Object *o) {
    call_state_2_handler_2684_unless_paused(o);
}

void load_velocities_from_table_4_slot04_00(Object *obj) {
    call_state_2_handler_268c_unless_paused(obj);
}

void load_velocities_from_table_5_slot04_00(Object *obj) {
    call_handler_2694_by_state_2(obj);
}

void call_state_1_table_handler_then_cull_check_slot04_00(Object *o) {
    data_801c056c_slot04_00[o->state_1](o);
    clear_or_set_flag_by_pos_x_limit(o);
}
