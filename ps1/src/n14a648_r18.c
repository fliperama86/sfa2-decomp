/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;

void func_80153c90(Effect *effect) {
    Object *object = (Object *)effect;
    object->field_00++;
}

void func_80153ca4(Effect *unused) {
}
