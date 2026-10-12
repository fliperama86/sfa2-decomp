/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void count_down_to_zero_then_advance_pass_slot28(Object *obj) {
    HudState *h;
    int t;
    if (obj->field_f0 == 0) {
        h = data_8018f5a0;
        t = h->countdown_timer - 1;
        h->countdown_timer = t;
        if ((s16)t == 0) {
            h->pass_index = h->pass_index + 1;
            start_fade_mode_1801_and_wait();
        }
    }
}
