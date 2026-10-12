/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Object *data_800519d8_slot28[];
void queue_rect_upload_job_slot28(Object *o);
void set_sequence_of_fourth_hud_object_7_slot28(Object *o, int n);

void advance_pass_on_countdown_zero_play_sound_slot28(Object *o) {
    HudState *h = data_8018f5a0;
    Object *p;
    int t = h->countdown_timer - 1;
    h->countdown_timer = t;
    if ((s16)t == 0) {
        h->countdown_timer = 0x258;
        h->pass_index++;
        p = data_800519d8_slot28[0];
        p->variant_index = 0xff;
        p = data_800519d8_slot28[1];
        p->variant_index = 0xff;
        queue_rect_upload_job_slot28(o);
        play_sound_at(0, 0, 0x305);
        set_sequence_of_fourth_hud_object_7_slot28(o, 5);
    }
}
