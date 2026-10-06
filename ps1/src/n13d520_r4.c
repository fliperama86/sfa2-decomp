/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_80141e34(Object *object) {
    if (*(u16 *)&object->field_04 == 1) return object->field_06 != 5;
    return 1;
}
