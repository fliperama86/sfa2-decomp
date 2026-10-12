/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c1f58_slot04_07[];

void call_handler_by_direction_index_slot04_07(Object *obj) {
    data_801c1f58_slot04_07[obj->direction_index >> 1](obj);
}
