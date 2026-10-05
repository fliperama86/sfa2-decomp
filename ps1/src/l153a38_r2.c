/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8015783c(int a, int b, int c, int d);

void func_80154364(int *p) {
    func_801575dc(p);
    p[0] = func_8015783c(0xf4000001, 4, 0x2000, 0);
    p[1] = func_8015783c(0xf4000001, 0x8000, 0x2000, 0);
    p[2] = func_8015783c(0xf4000001, 0x100, 0x2000, 0);
    p[3] = func_8015783c(0xf4000001, 0x2000, 0x2000, 0);
    p[4] = func_8015783c(0xf0000011, 4, 0x2000, 0);
    p[5] = func_8015783c(0xf0000011, 0x8000, 0x2000, 0);
    p[6] = func_8015783c(0xf0000011, 0x100, 0x2000, 0);
    func_8015786c();
    func_8015761c(p[0]);
    func_8015761c(p[1]);
    func_8015761c(p[2]);
    func_8015761c(p[3]);
    func_8015761c(p[4]);
    func_8015761c(p[5]);
    func_8015761c(p[6]);
}
