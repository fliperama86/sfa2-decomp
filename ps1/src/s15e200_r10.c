/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8015eff0(int a, void (*b)(void));

int func_8015f734(int a) {
    int old = data_80182f00;
    data_80182f00 = a;
    return old;
}

int func_8015f74c(void) {
    return data_80182f00;
}
