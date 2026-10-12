/* Reconstruction. Names/roles inferred, not original symbols.
   Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: set_table_columns_by_win_counts (original keeps only the left cf address in a
   register), adjust_counter_c_by_table_and_random (table byte lands in a0 in the original). */
#include "game.h"
#include "externs.h"
#include "protos.h"


int tick_counter_a_add_delta_8(GameState *state) {
    s16 value = counter_a;

    value--;
    counter_a = value;
    if (value == 0) {
        counter_a = 0x780;
        add_clamped_value(state, 8);
    }
    /* No return statement: declared without a result this function is one instruction shorter than the original, and the callee whose result the earlier source returned is defined without one. */
}
