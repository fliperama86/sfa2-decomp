/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051bdc_slot28;

void advance_pass_when_sequence_flag_negative_slot28(Object *obj) {
    if (menu_active_flag != 0 && *(s16 *)&data_80051bdc_slot28.p->step_flags < 0) {
        screen_state->countdown_timer = 0xb4;
        screen_state->pass_index++;
    }
}
