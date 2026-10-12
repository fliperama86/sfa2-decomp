/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void init_four_textured_screen_quads_slot28(Object *obj);
void link_two_quads_into_ordering_table_slot28(Object *obj);

void load_image_and_start_countdown_fixed_slot28(Object *obj) {
    HudState *h;
    load_image_from_stream((u8 *)0x800e0000);
    h = data_8018f5a0;
    h->countdown_timer = 0x12c;
    h->step_index_2 = h->step_index_2 + 1;
    init_four_textured_screen_quads_slot28(obj);
    start_fade_mode_c01_and_wait();
}

void update_countdown_then_advance_substep_slot28(Object *obj) {
    HudState *h;
    int t;
    link_two_quads_into_ordering_table_slot28(obj);
    h = data_8018f5a0;
    t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t < 0) {
        h->step_index_2 = 0;
        h->pass_index = 0;
        h->substep_index = h->substep_index + 1;
    }
}
