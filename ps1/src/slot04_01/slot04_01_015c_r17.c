/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_801bf254_slot04_01[])(Object *);
extern ObjectFn data_801bf268_slot04_01[];

void call_handler_by_state_1_slot04_01(Object *obj) {
    data_801bf254_slot04_01[obj->state_1](obj);
}

void call_state_1_table_handler_then_cull_check_slot04_01(Object *o) {
    data_801bf268_slot04_01[o->state_1](o);
    clear_or_set_flag_by_pos_x_limit(o);
}
