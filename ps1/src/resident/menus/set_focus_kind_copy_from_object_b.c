/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void set_focus_kind_copy_from_object_b(Attacker *unused) {
    game_state.focus_object->kind_copy = ~game_state.object_b->kind;
}
