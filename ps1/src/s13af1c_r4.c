/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013bbc4(void) {
    Object *tmp;
    if (ref_second.p->field_ad == 0 && ref_third.p->field_ad != 0) {
        func_8013b824();
    } else {
        tmp = ref_second.p;
        ref_second.p = ref_third.p;
        ref_third.p = tmp;
        func_8013b9e0();
        tmp = ref_second.p;
        ref_second.p = ref_third.p;
        ref_third.p = tmp;
    }
}

void func_8013bc64(void) {
    if (ref_third.p->field_ad != 0 || ref_second.p->field_ad == 0) {
        func_8013b9e0();
    } else {
        ref_third.p->field_00 = ref_third.p->field_00 + 1;
        ref_third.p->field_04 = 2;
        ref_third.p->field_05 = 0;
        ref_third.p->field_06 = 0;
        ref_third.p->field_07 = 0;
    }
}
