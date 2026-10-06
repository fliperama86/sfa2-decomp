/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern HudState *data_8018f5a0;

void func_80152404(Object *object) {
    object->field_09 = 4;
    object->field_0f = 1;
    object->field_04++;
    game_state.field_358 = object->field_3c;
    object->field_1c = game_state.field_358->field_1c;
}
