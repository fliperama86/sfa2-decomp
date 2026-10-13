/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051890_slot28;
extern ObjectRef data_80051894_slot28;
extern ObjectRef data_80051898_slot28;
extern ObjectRef data_8005189c_slot28;

void release_four_stored_objects_slot28(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)data_80051890_slot28.p);
    reset_and_push_object_to_pool_table((Slab172 *)data_80051894_slot28.p);
    reset_and_push_object_to_pool_table((Slab172 *)data_80051898_slot28.p);
    reset_and_push_object_to_pool_table((Slab172 *)data_8005189c_slot28.p);
}

void count_down_to_zero_then_advance_pass_gated_slot28(Object *obj) {
    HudState *h;
    int t;
    if (menu_active_flag != 0 && obj->field_f0 == 0) {
        h = screen_state;
        t = h->countdown_timer - 1;
        h->countdown_timer = t;
        if ((s16)t == 0) {
            h->pass_index = h->pass_index + 1;
            start_fade_mode_1801_and_wait();
        }
    }
}
