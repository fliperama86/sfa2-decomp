/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8002445c_slot12[])(Object *);
void set_phase_two_clear_step_slot12(Object *obj);

void update_substep_handler_and_blocks_slot12(Object *obj) {
    game_state.countdown_4 = game_state.countdown_4 - 1;
    data_8002445c_slot12[data_8018f5a0->substep_index](obj);
    update_blocks();
    update_active_slab172_entries();
}

void advance_substep_then_branch_on_field_ee_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    if (game_state.field_2bc == 1) {
        data_8018f5a0->substep_index = data_8018f5a0->substep_index + 1;
        if (obj->field_ee != 0) {
            set_phase_two_clear_step_slot12(o);
        } else {
            start_fade_mode_c01_and_wait();
        }
    }
}
