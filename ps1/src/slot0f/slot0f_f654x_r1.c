/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* The call of func_80125f5c passes its third argument as a signed halfword
   although the callee takes a byte: the original loads it with lh. Written
   as a plain call, this function differs from the original in 1 instruction
   slot. */
void func_800df654_slot0f(GameState *state) {
    int n = state->field_c4 - 1;
    state->field_c4 = n;
    if ((s16)n == 0) {
        state->field_c4 = 2;
        ((void (*)(u8, u8, s16, u8))func_80125f5c)(4, 0x20, state->field_c2, 0);
        func_80137220(4, 7);
        state->field_c2++;
        if ((s16)state->field_c2 >= 0x20) {
            data_8018f5a0->field_4a++;
        }
    }
}
