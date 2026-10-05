/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80119ddc(AnimObj *a) {
    if (a->field_38 < 0) {
        a->field_38 = 0;
    }
    func_80119e74(a, (data_80183bb0[a->field_94] - data_80183ad0[a->field_94]) / (a->field_38 + 1));
}
