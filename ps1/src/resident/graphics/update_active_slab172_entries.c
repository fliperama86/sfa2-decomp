/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_active_slab172_entries(void) {
    int i;
    data_801a4fe8 = (u8 *)data_801987cc[draw_buffer_index];
    data_801a697c = (u8 *)data_8018db18[draw_buffer_index];
    for (i = 0; i < 40; i++) {
        if (data_801a89f4[i].active_flag != 0 && data_801a89f4[i].enabled_flag != 0) {
            reload_stream_cells_then_build_sprite_quads((Slab172 *)&data_801a89f4[i]);
        }
    }
}
