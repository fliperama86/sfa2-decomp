/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void choose_pair_response_second_to_state_2(void) {
    Object *tmp;

    if (ref_second.p->active_flag_2 != 0 || ref_third.p->active_flag_2 == 0) {
        tmp = ref_second.p;
        ref_second.p = ref_third.p;
        ref_third.p = tmp;
        link_second_object_to_third_parent();
        tmp = ref_second.p;
        ref_second.p = ref_third.p;
        ref_third.p = tmp;
    } else {
        ref_second.p->active_count++;
        ref_second.p->state_0 = 2;
        ref_second.p->state_1 = 0;
        ref_second.p->state_2 = 0;
        ref_second.p->state_3 = 0;
    }
}
