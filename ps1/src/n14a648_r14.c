/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void clear_wait_flag_2(GameState *state) {
    state->wait_flag = 0;
}
