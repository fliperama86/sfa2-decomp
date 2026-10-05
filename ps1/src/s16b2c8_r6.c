/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8016d440(int a, unsigned b) {
    unsigned t;

    if (b > 0x7f000) {
        b = 0x7f000;
    }
    t = data_80183134 << data_8018315c;
    func_8016c850(a, b);
    data_80183134 = func_8016c988(-1, t + b);
    if (data_8018316c == 0) {
        data_80183168 = 0;
    }
    return b;
}
