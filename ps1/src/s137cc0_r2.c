/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: func_801384c4 (original keeps only the left cf address in a
   register), func_80138628 (table byte lands in a0 in the original). */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80138358(GameState *state) {
    int value = table_6cf0[state->field_2d];

    counter_a = 0x780;
    counter_b = 0;
    state->field_6e = value;
    counter_c = value << 3;
}

void func_8013839c(GameState *state) {
    if (state->mode == 3 && state->field_2f == 0) {
        func_801384c4(state);
    } else {
        func_801386f0(state);
        func_8013886c(state);
        func_8013860c(state);
        func_801387c0(state);
    }
}

void func_80138410(GameState *state) {
    counter_a = 0x780;
    if (state->mode == 3 && state->field_2f == 0) {
        func_801384c4(state);
    } else {
        if (player_left.field_cd == 0 && game_state.field_2f == 0) {
            player_left.field_cf = 0x10;
        }
        if (player_right.field_cd == 0 && game_state.field_2f == 0) {
            player_right.field_cf = 0x10;
        }
    }
}
