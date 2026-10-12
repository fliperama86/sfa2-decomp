/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_sequence_of_fourth_hud_object_17_slot28(Object *obj, int arg);

void advance_pass_index_and_set_sequence_0_16_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->pass_index = h->pass_index + 1;
    set_sequence_of_fourth_hud_object_17_slot28(obj, 0);
}
