/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80138290(void) {
    s32 i;
    Unit *unit = units_2c20;

    for (i = 0; i < 16; unit++, i++) {
        if (unit->field_00 != 0) {
            if (unit->field_66 == 0) {
                handlers_6bf0[unit->field_02](unit);
            } else {
                handlers_6c70[unit->field_02](unit);
            }
        }
    }
}
