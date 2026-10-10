/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in register choice and
 * instruction scheduling. The exact owner of the bytes in the PS1 build
 * stays the raw bytes of the module image; the build does not use this
 * file. The differential test next to it (difftest.py) compares the
 * behavior of this C with the original code on random inputs of the
 * contract below.
 *
 * What it does (inferred, not an original name): the end-of-step handler
 * of a menu-like state. It clears a byte flag, then acts only when the
 * hud's field_52 equals the game state's mode ORed with its field_07.
 * Then it counts the hud's field_4e up, sets game_state.field_09 and
 * calls func_801257f4 on the game state. If game_state.field_b0 is set
 * and bit 7 of field_b1 is clear, field_b1 becomes game_state.field_40,
 * except that field_b2 of 10 or 11 gives 0x13 or 0x12 instead (field_b2
 * of 0 leaves field_b1 as it is). Finally it clears game_state.field_c6
 * and the hud's field_50, field_52 and field_54, and moves field_4e one
 * further up (two when mode ORed with field_07 is 3, which also sets
 * field_c6 to 0x40).
 *
 * Contract:
 *   Argument: none. No return value.
 *   Reads: game_state.mode, field_07, field_b0, field_b1, field_b2; the hud
 *     pointer data_8018f5a0 and the hud's field_4e and field_52.
 *   Writes: data_8002f158_slot27 (a byte, set to 0); and, when the hud's
 *     field_52 matches: the hud's field_4e, field_50, field_52, field_54,
 *     game_state.field_09, field_40 (half, on the branch above), field_c6.
 *   Aliasing: the hud block and game_state are distinct.
 *   Callee replaced by a recorder, the same in both runs: func_801257f4
 *     (1 argument, the game state's address). The log watches, at every
 *     call, the whole game state (0x364 bytes), the whole hud block
 *     (0x64 bytes) and data_8002f158_slot27, so that a field written
 *     after the call instead of before it would show. The real callee's
 *     own effect on the game state is not known and not run; to test that
 *     the function reads mode and field_07 again after the call, the
 *     recorder stands for a callee that rewrites the words holding them
 *     (a store at the first call, as an interrupt would do).
 *   Excluded inputs: none.
 *   Not reached by any input: none expected.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u8 data_8002f158_slot27;
void func_801257f4(Select *sel);

void func_80010840_slot27(void) {
    int t;

    data_8002f158_slot27 = 0;
    if ((game_state.mode | game_state.field_07) == data_8018f5a0->field_52) {
        data_8018f5a0->field_4e++;
        game_state.field_09 = 1;
        func_801257f4((Select *)&game_state);
        if (game_state.field_b0 != 0 && (game_state.field_b1 & 0x80) == 0) {
            t = game_state.field_b1;
            if (game_state.field_b2 != 0) {
                if (game_state.field_b2 == 10) {
                    t = 0x13;
                }
                if (game_state.field_b2 == 11) {
                    t = 0x12;
                }
            }
            game_state.field_40 = t;
        }
        game_state.field_c6 = 0;
        data_8018f5a0->field_50 = 0;
        data_8018f5a0->field_52 = 0;
        data_8018f5a0->field_54 = 0;
        data_8018f5a0->field_4e++;
        if ((game_state.mode | game_state.field_07) == 3) {
            data_8018f5a0->field_4e++;
            game_state.field_c6 = 0x40;
        }
    }
}
