/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void increment_active_count(Effect *effect) {
    Object *object = (Object *)effect;
    object->active_count++;
}

void noop_effect_handler(Effect *unused) {
}
