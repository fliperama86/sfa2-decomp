/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801c2f24_slot04_0b[])(Object *, Object *);

void call_state_1_table_handler_then_margin_check_slot04_0b(Object *o, Object *p) {
    data_801c2f24_slot04_0b[o->state_1](o, p);
    clear_or_set_flag_by_pos_x_limit(o);
}
