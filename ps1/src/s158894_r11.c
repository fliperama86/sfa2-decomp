/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


int func_8015a560(u32 *a) {
    data_8018d3bc = a;
    return 0;
}

int func_8015a570(int a) {
    int r = func_8015a8c4(data_8018d3bc, a);
    if (r == -1) {
        return 0;
    }
    data_8018d3bc = data_8018d3bc + r;
    return a;
}

void func_8015a5d8(int a, int b) {
    data_8018d3cc = func_8015a9e4(a, b, &data_8018d3c8, &data_8018d3c0, &data_8018d3c4);
}
