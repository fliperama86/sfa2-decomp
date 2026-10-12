/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014f690(Menu *m, int a, int b) {
    s32 v = data_8017e9d4[a][b];
    u8 *p;
    int i;
    if (m->mode_index == 2) {
        while (!func_8015cc44(9, 0, 0)) {
        }
    }
    m->mode_index = 1;
    m->retry_count = 6;
    m->track_code = v;
    m->field_15 = 0;
    m->field_16 = 0;
    m->field_17 = 0;
    m->pending_flag = 0;
    m->field_1f = 0;
    i = 3;
    p = (u8 *)m + 3;
    for (; i >= 0; i--) {
        p[0x20] = 0;
        p--;
    }
    func_80150c6c(m, v);
    func_8015c990((int) func_8014f918);
    func_8014f7f8(m);
}
