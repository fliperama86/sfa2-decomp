/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8011e990(void *buf, int h) {
    int size;
    int off;
    func_80157d9c(0);
    off = h * 24;
    for (;;) {
        h = func_8015fde4(data_8016e6a0 + off, 0, 0);
        if (h == -1) continue;
        size = func_8015fe04(h, 0, 2);
        if (size == -1) continue;
        if (func_8015fe04(h, 0, 0) == -1) continue;
        if (size != func_8015fd24(h, buf, (size + 0x7ff) & -0x800)) continue;
        func_8015fe28(h);
        return size;
    }
}
