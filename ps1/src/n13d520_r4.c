/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_80141e34(Object *object) {
    if (*(u16 *)&object->state_0 == 1) return object->state_2 != 5;
    return 1;
}
