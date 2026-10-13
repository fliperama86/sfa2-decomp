/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051cd0_slot28[];

void fade_1a01_and_advance_pass_on_flag_2_slot28(Object *obj) {
    if (*(s16 *)&data_80051cd0_slot28[3]->step_flags < 0) {
        HudState *h = screen_state;
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1a01_and_wait();
    }
}
