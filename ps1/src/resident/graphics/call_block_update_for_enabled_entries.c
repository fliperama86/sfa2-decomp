/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void call_block_update_for_enabled_entries(void) {
    int i;
    for (i = 0; i < 16; i++) {
        if (data_801ac888[i].active_flag != 0 && data_801ac888[i].enabled_flag != 0) {
            reload_stream_cells_then_build_sprite_quads(&data_801ac888[i]);
        }
    }
}
