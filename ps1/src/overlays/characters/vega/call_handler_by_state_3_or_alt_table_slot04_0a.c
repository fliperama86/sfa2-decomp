/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c0560_slot04_0a[];
extern ObjectFn data_801c0568_slot04_0a[];

void call_handler_by_state_3_or_alt_table_slot04_0a(Object *obj) {
    if (obj->direction_index != 4) {
        data_801c0560_slot04_0a[obj->state_3](obj);
    } else {
        data_801c0568_slot04_0a[obj->state_3](obj);
    }
}
