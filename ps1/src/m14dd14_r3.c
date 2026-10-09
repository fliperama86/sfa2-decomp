/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8014f0bc(void) {
    Menu *m = &data_80190948;
    u8 buf[4];
    int i = 4;
    int ok = 0;
    buf[0] = 0x80;
    for (; i != 0; i--) {
        if (func_8015c6ec()) {
            ok = 1;
            break;
        }
    }
    func_8014f604(0);
    m->field_00 = 0;
    m->field_01 = 0;
    m->field_02 = 0;
    m->field_03 = 0;
    m->field_06 = 0;
    m->field_08 = 0;
    if (ok) {
        while (!func_8015cc44(0xe, (int)buf, 0)) {
        }
        m->field_28 = 0x10;
        func_8015cec4(0x10, &m->field_f0);
        while (!func_8015cc44(0x15, (int)&m->field_f0, 0)) {
        }
    }
    return ok;
}
