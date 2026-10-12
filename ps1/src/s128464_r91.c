/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void count_down_action_timer(Object *object) {
    if (object->action_timer != 0) object->action_timer--;
}
