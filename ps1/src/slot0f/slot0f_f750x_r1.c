/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The call of scale_palette_rows_all_entries passes its third argument as a signed halfword
   although the callee takes a byte: the original loads it with lh. Written
   as a plain call, this function differs from the original in 1 instruction
   slot. */
void func_800df750_slot0f(GameState *state) {
    int n = state->step_delay_timer - 1;
    state->step_delay_timer = n;
    if ((s16)n == 0) {
        state->step_delay_timer = 2;
        ((void (*)(u8, u8, s16, u8))scale_palette_rows_all_entries)(4, 0x20, state->step_counter, 0);
        func_80137220(4, 7);
        n = state->step_counter - 1;
        state->step_counter = n;
        if ((s16)n < 0) {
            data_8018f5a0->phase_index++;
        }
    }
}
