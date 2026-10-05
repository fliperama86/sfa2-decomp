/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011948c(void) {
    SndVoice *v = (SndVoice *)0x801fc200;
    for (; v <= (SndVoice *)0x801fc37f; v++) {
        if (v->field_00 == 1) {
            int n = v->field_02 - 1;
            v->field_02 = n;
            if ((n << 16) == 0) {
                v->field_00 = 2;
            }
        }
    }
}
