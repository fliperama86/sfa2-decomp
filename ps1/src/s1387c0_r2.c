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
    if ((short)player_left.field_5c < 0) {
        game_state.config->field_4c |= 2;
    }
    if ((short)player_right.field_5c < 0) {
        game_state.config->field_4c |= 1;
    }
}
