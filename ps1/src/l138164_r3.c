/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013b824(void) {
    ref_third.p->active_count++;
    ref_third.p->state_0 = 1;
    ref_third.p->state_1 = 1;
    ref_third.p->state_2 = 0;
    ref_third.p->state_3 = 0;
    ref_third.p->health = (s16)ref_third.p->health - 1;
    if ((s16)ref_third.p->health < 0) {
        ref_third.p->state_0 = 2;
        ref_third.p->state_1 = 0;
    }
    ref_second.p->active_count++;
    ref_second.p->state_0 = 1;
    ref_second.p->state_1 = 1;
    ref_second.p->state_2 = 0;
    ref_second.p->state_3 = 0;
    ref_second.p->health = (s16)ref_second.p->health - 1;
    if ((s16)ref_second.p->health < 0) {
        ref_second.p->state_0 = 2;
        ref_second.p->state_1 = 0;
    }
}
