/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void hit_both_objects_with_type_exception(void) {
    Object *o;
    u32 hi;

    ref_second.p->active_count += 1;
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
        o->health -= 1;
        if ((s16)ref_second.p->health < 0) {
            ref_second.p->state_0 = 2;
            ref_second.p->state_1 = 0;
        }
    }

    ref_third.p->active_count += 1;
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
        o->health -= 1;
        if ((s16)ref_third.p->health < 0) {
            ref_third.p->state_0 = 2;
            ref_third.p->state_1 = 0;
        }
    }
}
