/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051ac4_slot28[];

void advance_pass_when_sequence_ends_a_19_slot28(Object *obj) {
    if (*(s16 *)&data_80051ac4_slot28[3]->step_flags < 0) {
        HudState *h = screen_state;
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}
