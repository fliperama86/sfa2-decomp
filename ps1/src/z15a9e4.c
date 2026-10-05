/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8015a9e4(u32 *a, int b, int *c, int *d, int *e) {
    u32 *o = a + 3;
    if (func_80157c94() == 2) {
        func_8015a550(str_8016dd14);
    }
    if (func_80157c94() == 2) {
        func_8015a550(str_8016dd28, a[0], a[1], a[2], b);
    }
    if (func_80157c94() == 2) {
        func_8015a550(str_8016dd50, o[b * 7], o[b * 7 + 1]);
    }
    if (func_80157c94() == 2) {
        func_8015a550(str_8016dd68, o[b * 7 + 2], o[b * 7 + 3]);
    }
    if (func_80157c94() == 2) {
        func_8015a550(str_8016dd80, o[b * 7 + 4], o[b * 7 + 5]);
    }
    *d = (int) o + o[b * 7];
    *e = (int) o + o[b * 7 + 2];
    *c = (int) o + o[b * 7 + 4];
    return o[b * 7 + 5];
}
