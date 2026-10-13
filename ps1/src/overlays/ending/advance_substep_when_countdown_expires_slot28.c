/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void advance_substep_when_countdown_expires_slot28(Object *obj) {
    HudState *h = screen_state;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        h->substep_index = h->substep_index + 1;
    }
}
