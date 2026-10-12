/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051c90_slot28[];

void advance_pass_when_sequence_ends_a_29_slot28(Object *obj) {
    if (*(s16 *)&data_80051c90_slot28[3]->step_flags < 0) {
        HudState *h = data_8018f5a0;
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}
