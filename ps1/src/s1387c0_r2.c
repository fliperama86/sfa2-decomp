/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80138f74(void) {
    Object *pair = &player_left;

    func_80139a1c();
    func_80139304(pair, pair + 1);
    func_80139a1c();
    func_80139304(pair + 1, pair);
    if ((short)player_left.health < 0) {
        game_state.config->defeat_side_bits |= 2;
    }
    if ((short)player_right.health < 0) {
        game_state.config->defeat_side_bits |= 1;
    }
}
