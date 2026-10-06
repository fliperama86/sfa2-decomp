/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8011fe40(Object *object) {
    u32 *p;
    int z = 0;
    short i;
    game_state.cursor = table_80185cf4;
    if (object->side != 0) {
        game_state.cursor = table_80185e50;
    }
    i = 0x2a;
    do {
        i = i - 1;
        p = game_state.cursor;
        game_state.cursor = p + 1;
        *p = z;
    } while (i != -1);
    object->field_c2 = 0xf0fc;
    object->field_180 = z;
    object->field_182 = z;
    object->field_2a2 = z;
    object->field_2a1 = z;
    object->field_267 = 1;
}
