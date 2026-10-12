/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void set_selected_flag_by_index(u8 index) {
    data_801a6986 = 0;
    data_801a6987 = 0;
    data_801a6988 = 0;
    if (index == 0) {
        data_801a6986 = 1;
    } else if (index == 1) {
        data_801a6987 = 1;
    } else if (index == 2) {
        data_801a6988 = 1;
    }
}

void set_table_index_3_from_table(u8 index) {
    game_state.table_index_3 = table_8016e81c[index];
}

void reinit_round_and_copy_five_rows(void) {
    data_80197f1c = 0;
    ((HudBig *)data_8018f5a0)->substate_index = 0;
    game_state.field_65 = 0;
    clear_arrays_and_init_pools_and_sprite_tables();
    call_handler_by_kind_index_a();
    init_hud_primitives();
    spawn_position_table_objects_and_list_objects();
    copy_palette_rows_from_backup_bank(0, 0x20, 0);
    copy_palette_rows_from_backup_bank(1, 0x20, 0);
    copy_palette_rows_from_backup_bank(2, 0x20, 0);
    copy_palette_rows_from_backup_bank(3, 0x20, 0);
    copy_palette_rows_from_backup_bank(4, 0x20, 0);
    queue_five_jobs();
}
