/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: func_801384c4 (original keeps only the left cf address in a
   register), func_80138628 (table byte lands in a0 in the original). */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80138588(void) {
    player_left.field_cf = 0;
    player_right.field_cf = 0;
}

void func_801385a0(GameState *state) {
    if (!(state->mode == 3 && state->field_2f == 0)) {
        func_80138c98(state);
        func_801386a4(state);
        func_8013886c(state);
        func_8013860c(state);
        func_801387c0(state);
    }
}

void func_8013860c(GameState *state) {
    state->field_6e = (s16)counter_c >> 3;
}
