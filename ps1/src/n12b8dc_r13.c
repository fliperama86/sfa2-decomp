/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern ObjectRef ref_third;

void func_8013bdf4(void) {
    Object *o;
    u32 hi;

    ref_second.p->field_00 += 1;
    ref_second.p->field_04 = 1;
    ref_second.p->field_05 = 1;
    ref_second.p->field_06 = 0;
    ref_second.p->field_07 = 0;
    o = ref_second.p;
    hi = *(u32 *)o & 0xffff0000;
    if (hi == 0x4020000 || hi == 0x4140000) {
        o->field_04 = 2;
        ref_second.p->field_05 = 0;
    } else {
        o->field_5c -= 1;
        if ((s16)ref_second.p->field_5c < 0) {
            ref_second.p->field_04 = 2;
            ref_second.p->field_05 = 0;
        }
    }

    ref_third.p->field_00 += 1;
    ref_third.p->field_04 = 1;
    ref_third.p->field_05 = 1;
    ref_third.p->field_06 = 0;
    ref_third.p->field_07 = 0;
    o = ref_third.p;
    hi = *(u32 *)o & 0xffff0000;
    if (hi == 0x4020000 || hi == 0x4140000) {
        o->field_04 = 2;
        ref_third.p->field_05 = 0;
    } else {
        o->field_5c -= 1;
        if ((s16)ref_third.p->field_5c < 0) {
            ref_third.p->field_04 = 2;
            ref_third.p->field_05 = 0;
        }
    }
}
