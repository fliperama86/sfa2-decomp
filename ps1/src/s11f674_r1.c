/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void call_handler_by_side(Object *object, void *unused) {
    handlers_by_side[object->side](object);
}
