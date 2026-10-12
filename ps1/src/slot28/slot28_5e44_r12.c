/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_80051b04_slot28[];

void fade_1c01_and_advance_pass_on_flag_slot28(Object *obj) {
    if (*(s16 *)&data_80051b04_slot28[3]->step_flags < 0) {
        HudState *h = data_8018f5a0;
        h->pass_index = h->pass_index + 1;
        send_fade_command_1c01_and_wait();
    }
}
