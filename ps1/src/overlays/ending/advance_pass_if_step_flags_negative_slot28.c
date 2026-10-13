/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_8005198c_slot28;

void advance_pass_if_step_flags_negative_slot28(Object *obj) {
    if ((s16)data_8005198c_slot28.p->step_flags < 0) {
        screen_state->pass_index++;
    }
}
