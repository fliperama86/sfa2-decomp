/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051918_slot28;
extern int data_80051950_slot28;

void release_object_when_countdown_expires_slot28(Object *obj) {
    data_8018f5a0->countdown_timer = (s16)data_8018f5a0->countdown_timer - 1;
    if ((s16)data_8018f5a0->countdown_timer < 0) {
        reset_and_push_object_to_pool_table((Slab172 *)data_80051918_slot28.p);
        set_menu_command_by_code(6, 3);
        start_fade_mode_1_and_wait();
        data_8018f5a0->pass_index++;
    }
}

void set_flag_advance_substep_when_not_waiting_slot28(Object *obj) {
    if (game_state.wait_flag == 0) {
        data_80051950_slot28 = 1;
        data_8018f5a0->substep_index++;
    }
}
