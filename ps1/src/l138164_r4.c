/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8013b9e0(void) {
    ref_third.p->active_count++;
    ref_third.p->state_0 = 1;
    ref_third.p->state_1 = 2;
    ref_third.p->state_2 = 0;
    ref_third.p->state_3 = 0;
    ref_third.p->state_1 += ref_third.p->vel_table_index >> 1;
    *(Object **)((u8 *)ref_third.p + 0xa8) = ref_second.p;
    ref_second.p->active_count++;
    ref_second.p->state_0 = 1;
    ref_second.p->state_1 = 2;
    ref_second.p->state_2 = 0;
    ref_second.p->state_3 = 0;
    ref_second.p->state_1 += ref_third.p->vel_table_index >> 1;
    ref_second.p->team_id ^= 1;
    ref_first.p = ref_second.p->parent;
    ref_first.p->link_flags--;
    if (ref_first.p->link_flags == 0) {
        ref_first.p->linked_object_3 = 0;
    }
    ref_first.p = ref_third.p->parent;
    ref_first.p->linked_object_3 = (s32)ref_second.p;
    ref_first.p->link_flags++;
    ref_second.p->parent = ref_first.p;
}
