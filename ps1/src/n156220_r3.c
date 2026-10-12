/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void count_down_then_next_phase(void *a, Object *object) {
    int t = object->countdown_timer - 1;
    object->countdown_timer = t;
    if (t << 16 < 0) {
        object->phase_index = object->phase_index + 1;
    }
}
