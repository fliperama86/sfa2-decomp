/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800289f4_slot28[])(Object *);

void call_handler_by_state_index_table_a_slot28(Object *o) {
    Slot28Obj *obj = (Slot28Obj *)o;
    data_800289f4_slot28[obj->state_index & 0x7f](o);
}

void func_80013970_slot28(Object *obj) {
}

void advance_substep_and_set_active_flag_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->countdown_timer = 1;
    h->step_index_2 = 0;
    h->pass_index = 0;
    h->substep_index = h->substep_index + 1;
    obj->active_flag_2 = 1;
    clear_arrays_and_init_pools_and_sprite_tables();
}
