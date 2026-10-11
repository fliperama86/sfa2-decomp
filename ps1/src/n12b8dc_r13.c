/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013bdf4(void) {
    Object *o;
    u32 hi;

    ref_second.p->field_00 += 1;
    ref_second.p->state_0 = 1;
    ref_second.p->state_1 = 1;
    ref_second.p->state_2 = 0;
    ref_second.p->state_3 = 0;
    o = ref_second.p;
    hi = *(u32 *)o & 0xffff0000;
    if (hi == 0x4020000 || hi == 0x4140000) {
        o->state_0 = 2;
        ref_second.p->state_1 = 0;
    } else {
        o->field_5c -= 1;
        if ((s16)ref_second.p->field_5c < 0) {
            ref_second.p->state_0 = 2;
            ref_second.p->state_1 = 0;
        }
    }

    ref_third.p->field_00 += 1;
    ref_third.p->state_0 = 1;
    ref_third.p->state_1 = 1;
    ref_third.p->state_2 = 0;
    ref_third.p->state_3 = 0;
    o = ref_third.p;
    hi = *(u32 *)o & 0xffff0000;
    if (hi == 0x4020000 || hi == 0x4140000) {
        o->state_0 = 2;
        ref_third.p->state_1 = 0;
    } else {
        o->field_5c -= 1;
        if ((s16)ref_third.p->field_5c < 0) {
            ref_third.p->state_0 = 2;
            ref_third.p->state_1 = 0;
        }
    }
}
