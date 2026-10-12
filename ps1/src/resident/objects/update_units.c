/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_units(void) {
    s32 i;
    Unit *unit = units_2c20;

    for (i = 0; i < 16; unit++, i++) {
        if (unit->active != 0) {
            if (unit->handler_table_select == 0) {
                handlers_6bf0[unit->handler_index](unit);
            } else {
                handlers_6c70[unit->handler_index](unit);
            }
        }
    }
}
