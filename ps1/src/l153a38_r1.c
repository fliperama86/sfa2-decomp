/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80153a38(Actor *a) {
    if (a->owner->field_bc == 8) {
        func_80153b74(a);
        return;
    }
    if (a->target->field_6a < 2) {
        a->text_index = 8;
        func_80153d7c(a);
    } else {
        a->text_index = 9;
        func_80153d7c(a);
        a->number_value = a->target->field_6a;
        table_801803c4[a->side]->field_04 = table_80180340[a->side];
        func_80153cac(a);
    }
    func_80153f0c(a);
    if (a->owner->field_cd == 0) {
        func_80153ed8(a);
        if (a->owner->field_cd == 0) {
            table_8018049c[a->side]->field_06 = 0x60;
            func_80153dfc(a);
        }
    }
}
