/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"




void call_state_2_handler_1a90_then_bound_x(Object *object) {
    handlers_1a90[object->state_2](object);
    clamp_pos_x_to_bounds_and_push_other(object);
}
