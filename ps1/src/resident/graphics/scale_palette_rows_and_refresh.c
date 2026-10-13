/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void scale_palette_rows_and_refresh(void) {
    scale_palette_rows_from_backup_bank(0, 0x20, 8, 0);
    scale_palette_rows_from_backup_bank(1, 0x20, 8, 0);
    scale_palette_rows_from_backup_bank(2, 0x20, 8, 0);
    scale_palette_rows_from_backup_bank(3, 0x20, 8, 0);
    scale_palette_rows_from_backup_bank(4, 0x11, 8, 0);
    copy_palette_rows_from_backup_bank(4, 0xf, 0x11);
    scale_palette_rows_from_backup_bank(4, 1, 8, 0x18);
    scale_palette_rows_from_backup_bank(4, 1, 8, 0x1c);
    queue_five_jobs();
}

void call_substate_table_entry(void) {
    table_8016e804[((HudBig *)screen_state)->substate_index]();
}
