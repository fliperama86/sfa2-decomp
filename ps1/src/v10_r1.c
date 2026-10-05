/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8015c990(void (*f)(void));

void func_8014f690(Menu *m, int a, int b) {
    s32 v = data_8017e9d4[a][b];
    u8 *p;
    int i;
    if (m->field_00 == 2) {
        while (!func_8015cc44(9, 0, 0)) {
        }
    }
    m->field_00 = 1;
    m->field_0b = 6;
    m->field_18 = v;
    m->field_15 = 0;
    m->field_16 = 0;
    m->field_17 = 0;
    m->field_1c = 0;
    m->field_1f = 0;
    i = 3;
    p = (u8 *)m + 3;
    for (; i >= 0; i--) {
        p[0x20] = 0;
        p--;
    }
    func_80150c6c(m, v);
    func_8015c990(func_8014f918);
    func_8014f7f8(m);
}
