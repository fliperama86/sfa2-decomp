/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8011f674(Object *object, void *unused) {
    handlers_by_side[object->side](object);
}
