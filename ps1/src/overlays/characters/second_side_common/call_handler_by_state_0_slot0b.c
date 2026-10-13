/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*data_801e46bc_slot0b[])(Object *);

void call_handler_by_state_0_slot0b(Object *obj) {
    ref_game_state.p = (Object *)&game_state;
    data_801e46bc_slot0b[obj->state_0](obj);
}
