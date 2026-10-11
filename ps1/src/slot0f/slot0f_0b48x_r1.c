/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int scratch_word_08;
extern char data_800df100_slot0f[];
int func_800e0bf4_slot0f(int a, char *path);
int func_800e0a1c_slot0f(int mode);
void func_800e1348_slot0f(void);

/*
 * What it does (inferred, not an original name): a memory-card check that
 * runs once. When the scratchpad flag word (scratch_word_08) is 0 it calls
 * func_800e0bf4_slot0f with 3 and a text buffer; a non-zero answer selects
 * a mode (0 for the answers 1 and 3, 0x10 for any other) and
 * func_800e0a1c_slot0f is called with it; when that answers 0,
 * func_800e1348_slot0f is called and the result is 0. In every case the flag
 * is then set to 1, a byte of data_8016e686 is copied to game_state.field_10
 * and func_80120374 is called with it as a signed byte. The result is 1,
 * except 0 when func_800e0a1c_slot0f answered 0.
 */
int func_800e0b48_slot0f(void) {
    int ret = 1;
    int r;
    int mode;
    if (scratch_word_08 == 0) {
        r = func_800e0bf4_slot0f(3, data_800df100_slot0f);
        if (r != 0) {
            mode = 0x10;
            /* Stand-in, not recovered source: the assignment r = 1 (the two tests r == 3 and r == 1 in one, r == 1 || r == 3, are the plain form) reproduces the original's choice of registers (the compare against the register that holds 1). Without it this function differs from the original in 6 instruction slots. */
            if (r == 3) {
                r = 1;
            }
            if (r == 1) {
                mode = 0;
            }
            r = func_800e0a1c_slot0f(mode);
            /* Stand-in, not recovered source: the test below, never true and not shown by the original's instructions, reproduces the original's choice of registers (the answer r stays in a register of its own after the branch). Without it this function differs from the original in 4 instruction slots. */
            if (r == 0) {
                func_800e1348_slot0f();
                ret = 0;
            } else if (((u8)r & 0x100) != 0) {
                ret = 0;
            }
        }
    }
    scratch_word_08 = 1;
    game_state.field_10 = data_8016e686;
    func_80120374((s8)data_8016e686);
    return ret;
}
