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
            /* Written as the two tests r == 1 and r == 3 in one, this function differs from the original in 7 instruction lines. */
            if (r == 3) {
                r = 1;
            }
            if (r == 1) {
                mode = 0;
            }
            r = func_800e0a1c_slot0f(mode);
            /* Exception, not recovered source: the test below is never true (a byte has no bit 8); the original keeps the answer r in a register of its own after the branch (move v1,v0 before bnez v1), which only a later read of r gives, and combine reduces that read to nothing. Without it this function differs from the original in 4 instruction lines. */
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
