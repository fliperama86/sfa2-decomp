/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void call_handler_by_state_1_unless_gate_flag(Object *object) {
    if (game_state.gate_flag != 0) {
        object->state_0++;
    } else {
        table_801802c0[object->state_1](object);
        set_flag_and_push_if_mode_8(object);
    }
}
