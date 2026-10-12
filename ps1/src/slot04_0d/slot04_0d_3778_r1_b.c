/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c2aec_slot04_0d[];

void call_handler_by_substate_index_slot04_0d(Object *object) {
    data_801c2aec_slot04_0d[object->substate_index](object);
}
