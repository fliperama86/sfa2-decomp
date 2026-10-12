/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void choose_pair_handler_second_inactive(void) {
    Object *tmp;
    if (ref_second.p->active_flag_2 == 0 && ref_third.p->active_flag_2 != 0) {
        damage_second_and_third_objects();
    } else {
        tmp = ref_second.p;
        ref_second.p = ref_third.p;
        ref_third.p = tmp;
        link_second_object_to_third_parent();
        tmp = ref_second.p;
        ref_second.p = ref_third.p;
        ref_third.p = tmp;
    }
}

void choose_pair_response_third_to_state_2(void) {
    if (ref_third.p->active_flag_2 != 0 || ref_second.p->active_flag_2 == 0) {
        link_second_object_to_third_parent();
    } else {
        ref_third.p->active_count = ref_third.p->active_count + 1;
        ref_third.p->state_0 = 2;
        ref_third.p->state_1 = 0;
        ref_third.p->state_2 = 0;
        ref_third.p->state_3 = 0;
    }
}
