/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void play_sound_from_table_by_direction_index(Object *o) {
    play_sound_at(o, o->field_02, table_8016e6f0[(o->direction_index & 6) >> 1]);
}
