/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_8003c294_slot28[])(Object *);
void set_sequence_of_fourth_hud_object_8_slot28(Object *obj, int arg);
extern Object *data_80051a44_slot28[];

void call_pass_handler_and_update_blocks_a_7_slot28(Object *obj) {
    data_8003c294_slot28[data_8018f5a0->pass_index](obj);
    update_blocks();
    update_active_slab172_entries();
}

void advance_pass_index_and_set_sequence_0_8_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->pass_index = h->pass_index + 1;
    set_sequence_of_fourth_hud_object_8_slot28(obj, 0);
}

void advance_pass_when_sequence_ends_a_13_slot28(Object *obj) {
    if (*(s16 *)&data_80051a44_slot28[3]->step_flags < 0) {
        HudState *h = data_8018f5a0;
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}
