/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801257d0(Attacker *unused) {
    game_state.focus_object->kind_copy = ~game_state.object_b->kind;
}
