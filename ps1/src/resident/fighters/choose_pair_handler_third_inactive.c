/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void choose_pair_handler_third_inactive(void) {
    if (ref_third.p->active_flag_2 == 0 && ref_second.p->active_flag_2 != 0) {
        damage_second_and_third_objects();
    } else {
        link_second_object_to_third_parent();
    }
}
