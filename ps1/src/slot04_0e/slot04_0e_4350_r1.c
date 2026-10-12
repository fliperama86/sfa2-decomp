/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c60c0_slot04_0e[];

void call_handler_by_substate_index_slot04_0e(Object *obj) {
    data_801c60c0_slot04_0e[obj->substate_index](obj);
}
