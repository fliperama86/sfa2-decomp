/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801440e4(Object *object) {
    object->field_38 = 0xfff;
    if (game_state.config->field_76 != 0) {
        object->field_38 = 0x20;
        object->field_06 = object->field_06 + 1;
    }
}
