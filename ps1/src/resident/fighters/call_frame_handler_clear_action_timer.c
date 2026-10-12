/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void call_frame_handler_clear_action_timer(Object *a0, Object *object, Box32 *a2) {
    object->action_timer = 0;
    handlers_7018[object->frame->handler_index](a0, object, a2);
}
