/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80153b74(Actor *a) {
    if (a->field_1c->field_6a < 2) {
        a->field_08 = 0x17;
        func_80153d7c(a);
    } else {
        a->field_08 = 0x18;
        func_80153d7c(a);
        a->field_09 = a->field_1c->field_6a;
        table_801803c4[a->field_01]->field_04 = table_80180344[a->field_01];
        func_80153cac(a);
    }
    if (a->field_18->field_cd == 0) {
        func_80153ed8(a);
        if (a->field_18->field_cd == 0) {
            table_8018049c[a->field_01]->field_06 = 0x60;
            func_80153dfc(a);
        }
    }
}
