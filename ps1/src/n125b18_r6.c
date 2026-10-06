/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"

/* Own extern instead of externs.h: the original loads this byte signed (lb),
   externs.h declares it u8. */
extern s8 data_801a6988;
extern void (*table_80171988[])(Object *);

void func_80129c90(Object *object) {
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1 && data_801a6988 != 0) {
        object->field_130 = 0x1000;
        object->field_150 = 0x1000;
        object->field_c2 = 0x1000;
    }
    object->field_16a = 0;
    table_80171988[object->field_07](object);
}
