/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8016ced0(void (*cb)(void));
int func_8015783c(unsigned a, int b, int c, int d);
void func_8015f020(int a, void (*cb)(void));

int func_8016d0dc(int a, unsigned b) {
    if (b > 0x7f000) {
        b = 0x7f000;
    }
    func_8016c850(a, b);
    if (data_8018316c == 0) {
        data_80183168 = 0;
    }
    return b;
}

void func_8016d13c(int a) {
    if (a == 1) {
        data_80183168 = 0;
    } else {
        data_80183168 = 1;
    }
}
