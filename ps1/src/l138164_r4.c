/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013b9e0(void) {
    ref_third.p->field_00++;
    ref_third.p->state_0 = 1;
    ref_third.p->state_1 = 2;
    ref_third.p->state_2 = 0;
    ref_third.p->state_3 = 0;
    ref_third.p->state_1 += ref_third.p->field_ac >> 1;
    *(Object **)((u8 *)ref_third.p + 0xa8) = ref_second.p;
    ref_second.p->field_00++;
    ref_second.p->state_0 = 1;
    ref_second.p->state_1 = 2;
    ref_second.p->state_2 = 0;
    ref_second.p->state_3 = 0;
    ref_second.p->state_1 += ref_third.p->field_ac >> 1;
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
