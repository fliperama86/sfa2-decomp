/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


short func_8016a49c(int a, unsigned short b) {
    short i;
    if (b <= 16) {
        i = b;
        if (data_801a8994[i] == 2) {
            int h = table_801adfe8[i];
            func_8016d06c(0);
            func_8016d0a0(h);
            func_8016d0dc(a, table_801ad39c[i]);
            data_801a8994[i] = 1;
            return i;
        }
    }
    func_8016d13c(0);
    return -1;
}
