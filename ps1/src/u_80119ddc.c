/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80119ddc(AnimObj *a) {
    if (a->step_divisor < 0) {
        a->step_divisor = 0;
    }
    func_80119e74(a, (data_80183bb0[a->stream_index] - data_80183ad0[a->stream_index]) / (a->step_divisor + 1));
}
