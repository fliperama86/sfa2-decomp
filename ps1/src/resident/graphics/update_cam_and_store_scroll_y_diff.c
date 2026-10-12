/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void update_cam_and_store_scroll_y_diff(void) {
    Cam *cam = (Cam *)data_801904d8;
    int a, b;
    if (data_801904d8[4] == 0) {
        increment_cam_field_clear_field_and_load_sources(cam);
    } else if (data_801904d8[4] == 1) {
        load_cam_sources_and_call_kind_handler(cam);
    }
    a = ((u16 *)&cam->src_b)[1];
    b = game_state.scroll_y;
    a = (a - b) & 0x3ff;
    game_state.field_da = a;
}
