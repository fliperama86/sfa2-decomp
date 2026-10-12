/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_800e0b48_slot0f(void);

void func_800df6f8_slot0f(GameState *state) {
    data_8018f5a0->phase_index++;
    state->step_counter = 0x1f;
    state->step_delay_timer = 0x20;
    if (func_800e0b48_slot0f() == 0) {
        state->field_cc = 1;
    }
}
