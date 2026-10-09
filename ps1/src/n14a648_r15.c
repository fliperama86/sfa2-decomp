/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80152358(Object *object) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        func_80131094(object);
    }
    func_80120028(object);
}
