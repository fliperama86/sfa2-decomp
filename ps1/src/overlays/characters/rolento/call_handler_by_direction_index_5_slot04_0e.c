/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c60b4_slot04_0e[];

void call_handler_by_direction_index_5_slot04_0e(Object *obj) {
    data_801c60b4_slot04_0e[obj->direction_index >> 1](obj);
}
