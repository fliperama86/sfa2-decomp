/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_80015bf4_slot01[])(Object *obj);
extern void (*data_80015c00_slot01[])(Object *obj);

void call_handler_by_state_0_two_tables_slot01(Object *obj) {
    if (obj->table_index & 0x80) {
        data_80015c00_slot01[obj->state_0](obj);
    } else {
        data_80015bf4_slot01[obj->state_0](obj);
    }
}
