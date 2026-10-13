/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void advance_pass_when_countdown_hits_zero_2_slot28(Object *obj) {
    HudState *h = screen_state;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t == 0) {
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}
