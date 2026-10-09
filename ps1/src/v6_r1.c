/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8014f7f8(Menu *m) {
    u8 buf[4];
    int i;
    buf[0] = 0x80;
    if (m->field_1c != 0) {
        func_8015057c((Stream *)m);
        m->field_1c = 0;
    }
    func_8015cec4(m->field_28, &m->field_f0);
    m->field_0a = 0;
    m->field_09 = 0;
    m->field_17 = 1;
    if (m->field_0b == 6) {
        m->field_16 = 0;
    } else {
        m->field_16 = 1;
    }
retry:
    for (i = 7; i >= 0; i--) {
        m->entries[i].field_08 = 0;
    }
    while (!func_8015cc44(1, 0, &m->field_0c)) {
    }
    if (m->field_0c & 0x40) goto retry;
    while (func_8015c958(1, 0) != 0) {
    }
    if (!func_8015c9c0(0xe, buf, 0)) goto retry;
    if (!func_8015cc44(2, &m->field_f0, 0)) goto retry;
    if (!func_8015c9c0(6, 0, 0)) goto retry;
    m->field_0a = 1;
    m->field_14 = 0;
}
