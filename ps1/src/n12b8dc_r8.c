/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void tick_countdown_then_reset_to_state_1(Object *object) {
    int v = object->countdown;
    v -= 0x100;
    object->countdown = v;
    if ((v & 0xff00) == 0) {
        object->active_count = 1;
        object->state_0 = 1;
        object->state_1 = 0;
        object->state_2 = 0;
        object->state_3 = 0;
    }
}
