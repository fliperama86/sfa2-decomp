/*
 * Nonmatching. This function is NOT byte-identical to the original: the
 * built code differs from the original's bytes in instruction scheduling
 * and register choice. The exact owner of the bytes in the PS1 build stays
 * the raw bytes of the module image; the build does not use this file.
 * The differential test next to it (difftest.py) compares the behavior of
 * this C with the original code on random inputs of the contract below.
 *
 * What it does (inferred, not an original name): a memory-card check that
 * runs once. When the scratchpad flag word (scratch_word_08) is 0 it calls
 * func_800e0bf4_slot0f with 3 and a text buffer; a non-zero answer selects
 * a mode (0 for the answers 1 and 3, 0x10 for any other) and
 * func_800e0a1c_slot0f is called with it; when that answers 0,
 * func_800e1348_slot0f is called and the result is 0. In every case the flag
 * is then set to 1, a byte of data_8016e686 is copied to game_state.field_10
 * and func_80120374 is called with it as a signed byte. The result is 1,
 * except 0 when func_800e0a1c_slot0f answered 0.
 *
 * Contract (the roles named are inferred):
 *   No arguments. Result in v0: 1 or 0 as above.
 *   Reads: scratch_word_08, data_8016e686. Writes: scratch_word_08 (set to
 *     1) and the byte game_state.field_10.
 *   Callees, all replaced by recorders:
 *     func_800e0bf4_slot0f (2 arguments: 3 and the address of the text
 *       buffer data_800df100_slot0f, whose first 6 words are logged as the
 *       pointee), result chosen by the setup;
 *     func_800e0a1c_slot0f (1 argument), result chosen by the setup;
 *     func_800e1348_slot0f (no argument), result 0;
 *     func_80120374 (1 argument, the signed byte), result 0.
 *   Watched at every call: scratch_word_08 (1 word) and the word of
 *     game_state at offset 0x10, since the function writes them after some
 *     calls and before others, and the callees could read them.
 *   Not reached by any input: none expected; the test reports the slots.
 */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

/* Inferred declarations; the headers lack them. */
extern int scratch_word_08;
extern char data_800df100_slot0f[];
extern u8 data_8016e686;
int func_800e0bf4_slot0f(int a, char *path);
int func_800e0a1c_slot0f(int mode);
void func_800e1348_slot0f(void);

int func_800e0b48_slot0f(void) {
    int ret = 1;
    int r;
    int mode;

    if (scratch_word_08 == 0) {
        r = func_800e0bf4_slot0f(3, data_800df100_slot0f);
        if (r != 0) {
            mode = 0x10;
            if (r == 1 || r == 3) {
                mode = 0;
            }
            if (func_800e0a1c_slot0f(mode) == 0) {
                func_800e1348_slot0f();
                ret = 0;
            }
        }
    }
    scratch_word_08 = 1;
    game_state.field_10 = data_8016e686;
    func_80120374((s8)data_8016e686);
    return ret;
}
