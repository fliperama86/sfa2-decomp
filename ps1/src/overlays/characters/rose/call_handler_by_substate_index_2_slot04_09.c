/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801c7d6c_slot04_09[];

void call_handler_by_substate_index_2_slot04_09(Object *object) {
    data_801c7d6c_slot04_09[object->substate_index](object);
}
