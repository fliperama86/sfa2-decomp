/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80139d04(Object *a0, Object *object, Box32 *a2) {
    object->field_29f = 0;
    handlers_7018[object->frame->field_06](a0, object, a2);
}
