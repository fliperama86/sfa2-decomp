/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_8014f76c(Ctl *p) {
    int i;
    for (i = 0; i < 4; i++) {
        if (p->slots[i]) {
            func_801203b4(i);
        }
    }
    p->field_0b = 6;
    p->field_15 = 0;
    p->field_16 = 0;
    p->field_17 = 0;
    p->field_1c = 0;
    p->field_1f = 0;
    func_80150c6c(p, p->field_18);
    func_8014f7f8(p);
}
