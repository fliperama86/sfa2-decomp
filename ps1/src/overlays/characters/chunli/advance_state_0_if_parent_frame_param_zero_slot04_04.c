/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void advance_state_0_if_parent_frame_param_zero_slot04_04(Object *obj) {
    if (obj->parent->frame->frame_param == 0) {
        obj->state_0 = obj->state_0 + 1;
    }
}

void reset_states_and_health_slot04_04(Object *obj) {
    obj->state_0 = 1;
    obj->active_count = 1;
    obj->state_1 = 0;
    obj->state_2 = 0;
    obj->state_3 = 0;
    obj->health = 0xff;
}
