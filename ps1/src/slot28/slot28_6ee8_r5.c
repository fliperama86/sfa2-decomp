/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80017944_slot28(Object *obj, int arg);
extern Object *data_8005193c_slot28[];

void func_800174fc_slot28(Object *obj) {
    HudState *h = data_8018f5a0;
    h->pass_index = h->pass_index + 1;
    func_80017944_slot28(obj, 0);
}

void func_80017530_slot28(Object *obj) {
    if (*(s16 *)&data_8005193c_slot28[3]->step_flags < 0) {
        HudState *h = data_8018f5a0;
        h->pass_index = h->pass_index + 1;
        start_fade_mode_1801_and_wait();
    }
}
