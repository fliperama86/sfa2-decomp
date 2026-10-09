/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013bd10(void) {
    Object *tmp;

    if (ref_second.p->field_ad != 0 || ref_third.p->field_ad == 0) {
        tmp = ref_second.p;
        ref_second.p = ref_third.p;
        ref_third.p = tmp;
        func_8013b9e0();
        tmp = ref_second.p;
        ref_second.p = ref_third.p;
        ref_third.p = tmp;
    } else {
        ref_second.p->field_00++;
        ref_second.p->field_04 = 2;
        ref_second.p->field_05 = 0;
        ref_second.p->field_06 = 0;
        ref_second.p->field_07 = 0;
    }
}
