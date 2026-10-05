/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8016ced0(void (*cb)(void));
int func_8015783c(unsigned a, int b, int c, int d);
void func_8015f020(int a, void (*cb)(void));

void func_8016ce4c(void) {
    int h;

    if (data_801835fc == 0) {
        data_801835fc = 1;
        func_801575dc();
        data_80183190 = 0;
        func_8016ced0(func_8016c20c);
        h = func_8015783c(0xf0000009, 0x20, 0x2000, 0);
        data_801831ec = h;
        func_8015761c(h);
        func_8015786c();
    }
}

void func_8016ced0(void (*cb)(void)) {
    func_8015f020(4, cb);
}
