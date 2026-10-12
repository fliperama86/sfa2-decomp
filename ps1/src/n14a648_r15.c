/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80152358(Object *object) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        advance_sequence_checked(object);
    }
    func_80120028(object);
}
