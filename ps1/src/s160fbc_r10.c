/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80164e88(int a, int b, unsigned short c, unsigned short d) {
    func_80163234(a | (b << 8), c, d, 0);
}

void func_80164ec0(short a, unsigned short b, unsigned short c) {
    func_80163234(a, b, c, 0);
}

void func_80164ef0(int a, int b, unsigned short c, unsigned short d) {
    func_80163234(a | (b << 8), c, d, 0);
}
