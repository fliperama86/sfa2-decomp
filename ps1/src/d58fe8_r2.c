/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"

int func_8015fe38(int a, int b, int c, u8 *d);

int func_8015fd24(int a, u8 *b, unsigned int c) {
    int total;
    int n;
    int r;
    for (total = 0; c != 0; ) {
        n = c;
        if (c > 0x8000) {
            n = 0x8000;
        }
        r = func_8015fe38(0, a, n, b);
        total += r;
        if (r == -1) {
            return -1;
        }
        b += r;
        c -= r;
        if (r < n) {
            return total;
        }
    }
    return total;
}
