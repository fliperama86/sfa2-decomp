/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern ObjectRef ref_third;

void func_8013b9e0(void) {
    ref_third.p->field_00++;
    ref_third.p->field_04 = 1;
    ref_third.p->field_05 = 2;
    ref_third.p->field_06 = 0;
    ref_third.p->field_07 = 0;
    ref_third.p->field_05 += ref_third.p->field_ac >> 1;
    *(Object **)((u8 *)ref_third.p + 0xa8) = ref_second.p;
    ref_second.p->field_00++;
    ref_second.p->field_04 = 1;
    ref_second.p->field_05 = 2;
    ref_second.p->field_06 = 0;
    ref_second.p->field_07 = 0;
    ref_second.p->field_05 += ref_third.p->field_ac >> 1;
    ref_second.p->field_65 ^= 1;
    ref_first.p = ref_second.p->field_3c;
    ref_first.p->field_240--;
    if (ref_first.p->field_240 == 0) {
        ref_first.p->field_14c = 0;
    }
    ref_first.p = ref_third.p->field_3c;
    ref_first.p->field_14c = (s32)ref_second.p;
    ref_first.p->field_240++;
    ref_second.p->field_3c = ref_first.p;
}
