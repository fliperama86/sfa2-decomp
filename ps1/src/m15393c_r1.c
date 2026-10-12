/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8015393c(Actor *a) {
    if (a->target->field_6a >= 2) {
        a->text_index = 7;
        func_80153d7c(a);
        a->number_value = a->target->field_6a;
        table_801803c4[a->side]->field_04 = table_8018033c[a->side];
        func_80153cac(a);
        if (a->owner->field_cd == 0) {
            func_80153ed8(a);
            if (a->owner->field_cd == 0) {
                table_8018049c[a->side]->field_06 = 0x50;
                func_80153dfc(a);
            }
        }
    } else {
        a->field_02 = 0;
    }
}
