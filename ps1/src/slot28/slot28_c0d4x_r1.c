/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800519d8_slot28[];
extern ObjectRef data_80051a10_slot28;
void queue_two_rect_upload_jobs_slot28(Object *o);

void advance_pass_when_sequence_ends_set_timer_slot28(Object *o) {
    Object *p = data_80051a10_slot28.p;
    if ((s16)p->step_flags < 0) {
        HudState *h = data_8018f5a0;
        h->countdown_timer = 0xf0;
        h->pass_index++;
        p = data_800519d8_slot28[0];
        p->variant_index = 0xff;
        queue_two_rect_upload_jobs_slot28(o);
    }
}
