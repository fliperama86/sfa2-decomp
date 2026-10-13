/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern ObjectRef data_80051a10_slot28;
extern Object *data_800519d8_slot28[];
extern u16 data_80051eb8_slot28[];

void advance_pass_index_and_fade_on_negative_flag_slot28(Object *obj) {
    Object *p = data_80051a10_slot28.p;
    if (((Slot28Obj *)p)->field_3a < 0) {
        HudState *h = screen_state;
        h->pass_index = h->pass_index + 1;
        p = data_800519d8_slot28[2];
        p->state_0 = 2;
        data_80051eb8_slot28[0] = 0xfff;
        start_fade_command_2a01_1f00_if_idle();
    }
}
