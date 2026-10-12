/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80139d04(Object *a0, Object *object, Box32 *a2) {
    object->action_timer = 0;
    handlers_7018[object->frame->handler_index](a0, object, a2);
}
