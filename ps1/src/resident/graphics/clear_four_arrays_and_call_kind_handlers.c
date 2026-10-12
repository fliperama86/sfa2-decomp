/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void clear_four_arrays_and_call_kind_handlers(void) {
    data_801aa548[0] = 0;
    data_801aa548[1] = 0;
    data_801aa548[2] = 0;
    data_801aa548[3] = 0;
    data_801aa5d8[0] = 0;
    data_801aa5d8[1] = 0;
    data_801aa5d8[2] = 0;
    data_801aa5d8[3] = 0;
    data_801aa668[0] = 0;
    data_801aa668[1] = 0;
    data_801aa668[2] = 0;
    data_801aa668[3] = 0;
    data_801904dc[0] = 0;
    data_801904dc[1] = 0;
    data_801904dc[2] = 0;
    data_801904dc[3] = 0;
    call_handler_by_kind_index_a();
    table_80172550[game_state.kind_index]();
}
