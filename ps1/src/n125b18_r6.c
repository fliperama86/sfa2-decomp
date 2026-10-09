/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* The original loads data_801a6988 with a signed load; the shared declaration
   is unsigned, so the read says so. */

void func_80129c90(Object *object) {
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1 && *(s8 *)&data_801a6988 != 0) {
        object->field_130 = 0x1000;
        object->field_150 = 0x1000;
        object->field_c2 = 0x1000;
    }
    object->field_16a = 0;
    table_80171988[object->field_07](object);
}
