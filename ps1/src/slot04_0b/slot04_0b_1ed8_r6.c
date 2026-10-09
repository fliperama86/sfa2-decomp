/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b2510_slot04_0b(Object *o) {
    *(s32 *)&o->field_10 += o->field_4c;
    *(s32 *)&o->field_14 -= o->field_50;
    o->field_50 += o->field_58;
    return o->pos_y <= o->field_70;
}
