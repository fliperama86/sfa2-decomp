/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011948c(void) {
    SndVoice *v = (SndVoice *)0x801fc200;
    for (; v <= (SndVoice *)0x801fc37f; v++) {
        if (v->status == 1) {
            int n = v->delay_timer - 1;
            v->delay_timer = n;
            if ((n << 16) == 0) {
                v->status = 2;
            }
        }
    }
}
