/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): one step of a count-up
 * animation that draws a row of sprites; the counterpart of
 * func_800df750_slot0f. A tick counter (field_c4) is decremented; when it
 * reaches 0 it is reloaded with 2, a draw call is made with the current
 * frame (field_c2) and a second call follows; the frame is then incremented,
 * and when it reaches 0x20 or more (as a signed 16-bit value) the step
 * counter of the HUD state (field_4a) is incremented, which moves the
 * sequence on.
 *
 * Contract (the roles named for the fields are inferred):
 *   Argument: a0 = pointer to a GameState. No return value.
 *   Reads and writes: state->field_c4 (u16, decremented every call), and
 *     when that reaches 0: state->field_c4 (set to 2), state->field_c2 (u16,
 *     incremented) and data_8018f5a0->field_4a (u16, incremented when the
 *     incremented field_c2 is 0x20 or more as a signed 16-bit value).
 *   Calls, in this order, when the tick counter reaches 0: func_80125f5c
 *     (4, 0x20, frame, 0), then func_80137220 (4, 7). The frame is taken as
 *     the signed 16-bit field_c2 and the callee reads its low byte. Both are
 *     replaced by recorders returning 0. The recorder of func_80125f5c
 *     logs the third argument under the mask 0xff (the callee takes a u8).
 *   Watched at every call: the whole GameState block (217 words) and the
 *     whole HudState block (25 words), because the function writes
 *     field_c4 before the first call and field_c2 after the second, and the
 *     callees could read them.
 *   Aliasing: the GameState block and the HudState block are distinct.
 *   Not reached by any input: none expected; the test reports the slots.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
void func_80125f5c(u8 bank, u8 count, u8 scale, u8 row);

void func_800df654_slot0f(GameState *state) {
    state->field_c4--;
    if (state->field_c4 == 0) {
        state->field_c4 = 2;
        func_80125f5c(4, 0x20, (s16)state->field_c2, 0);
        func_80137220(4, 7);
        state->field_c2++;
        if ((s16)state->field_c2 >= 0x20) {
            data_8018f5a0->field_4a++;
        }
    }
}
