/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: func_801384c4 (original keeps only the left cf address in a
   register), func_80138628 (table byte lands in a0 in the original). */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80138838(GameState *state, int delta);

int func_801386a4(GameState *state) {
    s16 value = counter_a;

    value--;
    counter_a = value;
    if (value == 0) {
        counter_a = 0x780;
        func_80138838(state, 8);
    }
    /* No return statement: declared without a result this function is one instruction shorter than the original, and the callee whose result the earlier source returned is defined without one. */
}
