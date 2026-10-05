/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Aliases of player_left.field_b2 (read as u8) and player_right.field_b2 (read as s16); the member width differs. */

Object *func_80125060(Object *object) {
    Object *result = &player_left;
    if (object->field_08 != 3) {
        if ((object->field_08 & 1) == 0) {
            result++;
        }
    } else if (data_80198406 > data_80198072) {
        result++;
    }
    return result;
}
