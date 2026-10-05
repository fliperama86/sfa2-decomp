/* Reconstruction. Names/roles inferred, not original symbols.
 * Historical note, from before this unit was exact or about a function that is no longer in this unit: Residuals: func_8014edac (original keeps game_state base in a saved
 * register from before the third func_801203b4 call, and tests/clears through
 * it; tried a local g set at top, set mid-function, ternary and if/else for
 * the argument). func_8014f038 (original derives both player addresses from
 * the field_cd address in one register; tried if/else, ternary, p++,
 * default-then-override). */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014ec80(int a, int b) {
    func_8014f3b8(1, a * 2);
    func_8014f3b8(1, b * 2 | 1);
    data_801a27d8 = 0x80010000;
    if (a == b) {
        data_801a27dc = 0x80010000;
    } else {
        data_801a27dc = 0x800fb100 - data_8017d918[b];
    }
}

void func_8014ed04(void) {
    func_8014f3b8(1, 0x2a);
    data_801a27d8 = data_801a27dc = 0x80010000;
}

void func_8014ed3c(void) {
    func_8014f3b8(1, 0x2b);
    data_801a27d8 = data_801a27dc = 0x80010000;
}

void func_8014ed74(void) {
    func_8014f3b8(1, 0x2c);
    data_801a27d8 = data_801a27dc = 0x80010000;
}
