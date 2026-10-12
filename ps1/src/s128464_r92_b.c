/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80129c40(Object *object) {
    if (object->hold_timer != 0) object->hold_timer--;
}

void decrement_cooldown_timer(Object *object) {
    if (object->cooldown_timer != 0) object->cooldown_timer--;
}
