/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void advance_sequence_rec_by_group_unless_paused(Object *object) {
    if (game_state.field_65 == 0 && game_state.field_74 == 0) {
        if (object->field_0e == 4) {
            advance_sequence_rec(object);
        } else if (object->field_0e == 8) {
            advance_sequence_rec_2(object);
        } else if (object->field_0e == 0xc) {
            advance_sequence_rec_3(object);
        }
    }
}
