/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8016a49c(int, int);

void func_8016a3b0(unsigned short a) {
    short i;
    if (a < 16) {
        i = a;
        if (data_801a8994[i] == 1) {
            func_8016cc18(table_801adfe8[i]);
            data_801a8994[i] = 0;
            data_801adfe0 = data_801adfe0 - 1;
        }
    }
}

short func_8016a440(int a) {
    short i;
    int r;
    i = r = func_80165850(a, -1);
    if (i != -1) {
        r = func_8016a49c(table_801ae0d8[i], i);
    }
    return r;
}
