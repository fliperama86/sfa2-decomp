/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_800518c8_slot28;

void start_countdown_180_when_step_flags_negative_slot28(Object *obj) {
    if ((s16)data_800518c8_slot28.p->step_flags < 0) {
        screen_state->countdown_timer = 0xb4;
        screen_state->pass_index = screen_state->pass_index + 1;
    }
}
