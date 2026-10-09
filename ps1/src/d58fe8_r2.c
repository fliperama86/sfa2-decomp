/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "protos.h"

int func_8015fd24(int a, u8 *b, unsigned int c) {
    int total;
    int n;
    int r;
    total = 0;
    while (c != 0) {
        n = c;
        if (c > 0x8000) {
            n = 0x8000;
        }
        r = func_8015fe38(0, a, n, b);
        if (r == -1) {
            return -1;
        }
        total += r;
        b += r;
        c -= r;
        if (r < n) {
            return total;
        }
    }
    return total;
}
