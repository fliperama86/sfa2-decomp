/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051a98_slot28[];

void count_down_hud_timer_then_advance_pass_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t == 0) {
        h->countdown_timer = 0x12c;
        h->pass_index++;
        data_80051a98_slot28[1]->variant_index = 0xff;
    }
}
