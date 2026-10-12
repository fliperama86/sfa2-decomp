/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_of_fourth_hud_object_4_slot28(Object *obj, int arg);
extern Object *data_8005193c_slot28[];

void advance_pass_index_and_set_sequence_0_4_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->pass_index = h->pass_index + 1;
    set_sequence_of_fourth_hud_object_4_slot28(obj, 0);
}

void advance_pass_when_sequence_ends_a_7_slot28(Object *obj) {
    if (*(s16 *)&data_8005193c_slot28[3]->step_flags < 0) {
        HudState *h = data_8018f5a0;
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}
