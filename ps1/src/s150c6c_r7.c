/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801519b4(Object *object) {
    if (game_state.field_74 == 0) {
        if (data_8018d204 < 0x30) {
            table_8018d144[data_8018d204++] = object;
        }
    }
}
