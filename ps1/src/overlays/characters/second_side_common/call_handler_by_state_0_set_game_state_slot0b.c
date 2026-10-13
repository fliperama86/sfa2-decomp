/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern void (*table_801e4cd0_slot0b[])(Object *);

void call_handler_by_state_0_set_game_state_slot0b(Object *obj) {
    ref_game_state.p = (Object *)&game_state;
    table_801e4cd0_slot0b[obj->state_0](obj);
}
