/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

u8 func_8013d210(Object *object) {
    return *((u8 *)&object->field_134 + 1) & 1;
}
