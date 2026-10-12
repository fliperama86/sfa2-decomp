/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void select_buffers_and_run_first_setup(void) {
    int n = data_801a27d0;
    data_801a4fe8 = (u8 *)data_801987cc[n];
    data_801a697c = (u8 *)data_8018db18[n];
    data_801ad3dc = (u8 *)&data_801987cc[n][0x280];
    data_801ae118 = (u8 *)data_8019056c[n];
    if (game_state.setup_done_flag == 0) {
        add_selector_hud_prims_and_strips();
        set_part_modes_and_update_both_player_parts();
        draw_all_block172_entries();
        update_enabled_units_by_handler_index();
        call_handler_by_mode_for_active_entries();
        call_kind_handler_if_flag_set();
        game_state.setup_done_flag = 1;
        data_801a695c = 0;
    }
    data_801aa540 = 0;
    copy_three_globals_and_clear_three();
}
