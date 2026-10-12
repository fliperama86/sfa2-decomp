/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801565b8(void *a, Object *object) {
    s16 t = *(s16 *)&object->countdown_timer;
    if (t != 0) {
        object->countdown_timer = t - 1;
    } else {
        object->countdown_timer = 600;
        object->phase_index = object->phase_index + 1;
    }
}
