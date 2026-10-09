/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013b824(void) {
    ref_third.p->field_00++;
    ref_third.p->field_04 = 1;
    ref_third.p->field_05 = 1;
    ref_third.p->field_06 = 0;
    ref_third.p->field_07 = 0;
    ref_third.p->field_5c = (s16)ref_third.p->field_5c - 1;
    if ((s16)ref_third.p->field_5c < 0) {
        ref_third.p->field_04 = 2;
        ref_third.p->field_05 = 0;
    }
    ref_second.p->field_00++;
    ref_second.p->field_04 = 1;
    ref_second.p->field_05 = 1;
    ref_second.p->field_06 = 0;
    ref_second.p->field_07 = 0;
    ref_second.p->field_5c = (s16)ref_second.p->field_5c - 1;
    if ((s16)ref_second.p->field_5c < 0) {
        ref_second.p->field_04 = 2;
        ref_second.p->field_05 = 0;
    }
}
