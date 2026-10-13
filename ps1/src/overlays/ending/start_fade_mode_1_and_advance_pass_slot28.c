/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051c1c_slot28;

void start_fade_mode_1_and_advance_pass_slot28(Object *obj) {
    if (((Slot28Obj *)data_80051c1c_slot28.p)->field_3a < 0) {
        start_fade_mode_1_and_wait();
        screen_state->pass_index = screen_state->pass_index + 1;
    }
}
