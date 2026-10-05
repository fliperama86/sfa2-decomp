/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern u16 data_801a6966;

void func_80119718(void) {
    int cur;
    int old;
    int oldnew;
    cur = 0;
    if (data_801a89a8[0] == 0 && data_801a89a8[1] == 0x41) {
        cur = ~(data_801a89a8[3] | (data_801a89a8[2] << 8));
    }
    old = data_801a6966;
    oldnew = data_801a696a;
    data_801a6966 = cur;
    data_801a6968 = old;
    data_801a696c = oldnew;
    data_801a696a = cur & ~old;
    if (data_801a89a8[4] == 0 && data_801a89a8[5] == 0x41) {
        cur = ~(data_801a89a8[7] | (data_801a89a8[6] << 8));
    } else {
        cur = 0;
    }
    old = data_801a6972;
    oldnew = data_801a6976;
    data_801a6972 = cur;
    data_801a6974 = old;
    data_801a6976 = cur & ~old;
    data_801a6978 = oldnew;
    if (data_801a6976 & 0x800) {
        data_801ac61c = ~data_801ac61c & 1;
    }
}
