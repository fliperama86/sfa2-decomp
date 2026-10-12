/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_800519d8_slot28[];
void func_8001c3ac_slot28(Object *o);
void func_8001c430_slot28(Object *o, int n);

void func_8001c13c_slot28(Object *o) {
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
        func_8001c3ac_slot28(o);
        play_sound_at(0, 0, 0x305);
        func_8001c430_slot28(o, 5);
    }
}
