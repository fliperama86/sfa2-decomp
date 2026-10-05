/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern ObjectRef ref_third;

void func_8013b97c(void) {
    if (ref_third.p->field_ad == 0 && ref_second.p->field_ad != 0) {
        func_8013b824();
    } else {
        func_8013b9e0();
    }
}
