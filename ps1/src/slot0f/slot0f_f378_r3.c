/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_80125f5c(u8 bank, u8 count, u8 scale, u8 row);
int func_800e0b48_slot0f(void);

void func_800df6f8_slot0f(GameState *state) {
    data_8018f5a0->field_4a++;
    state->field_c2 = 0x1f;
    state->field_c4 = 0x20;
    if (func_800e0b48_slot0f() == 0) {
        state->field_cc = 1;
    }
}
