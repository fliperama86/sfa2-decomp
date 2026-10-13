/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot04SelRec9d78 data_801b9d78_slot04_sel[];

void remap_selection_indices_by_side_mask_slot04_sel(void) {
    Slot04SelRec9d78 *r = data_801b9d78_slot04_sel + 1;
    if (game_state.side_mask & 1) {
        if (data_801b9d78_slot04_sel[0].selection_index == 0x12) {
            data_801b9d78_slot04_sel[0].selection_index = 4;
        }
        if (data_801b9d78_slot04_sel[0].selection_index == 0x13) {
            data_801b9d78_slot04_sel[0].selection_index = 0x11;
        }
        if (data_801b9d78_slot04_sel[0].selection_index == 0x14) {
            data_801b9d78_slot04_sel[0].selection_index = 2;
        }
    }
    if (game_state.side_mask & 2) {
        if (r->selection_index == 0x12) {
            r->selection_index = 4;
        }
        if (r->selection_index == 0x13) {
            r->selection_index = 0x11;
        }
        if (r->selection_index == 0x14) {
            r->selection_index = 2;
        }
    }
    screen_state->step_index_2 = screen_state->step_index_2 + 1;
}
