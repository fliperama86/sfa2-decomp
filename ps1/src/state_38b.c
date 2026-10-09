/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Second name for game_state. The original loads this address on its own,
   apart from the store through game_state; with one name this compiler
   derives it from the store address. */


void func_80138ec4(void) {
    game_state.config = &game_state_second;
    if (game_state.config->field_a8 == 0 &&
        (player_left.field_165 | player_right.field_165) == 0) {
        func_80138f74();
        func_8013b558();
        func_8013bfa4();
        if (game_state.field_30 != 0 && *(u16 *)data_801a6984 == 0) {
            player_left.field_5c = 0x90;
            player_right.field_5c = 0x90;
        }
    }
}
