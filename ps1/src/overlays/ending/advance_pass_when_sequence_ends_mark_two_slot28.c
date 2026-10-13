/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_80051a98_slot28[];
extern ObjectRef data_80051ad0_slot28;

void advance_pass_when_sequence_ends_mark_two_slot28(Object *obj) {
    Object *p = data_80051ad0_slot28.p;
    if ((s16)p->step_flags < 0) {
        screen_state->countdown_timer = 0xb4;
        screen_state->pass_index++;
        p = data_80051a98_slot28[0];
        p->variant_index = 0xff;
        p = data_80051a98_slot28[1];
        p->variant_index = 0xff;
    }
}
