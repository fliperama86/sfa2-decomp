/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void init_screen_tables_and_hide_display_slot01(void) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    int unused[2];
    init_sprite_tables();
    data_8018f5a0->substep_index++;
    data_801aa65e[0] = 1;
    data_80190562[0] = 1;
    game_state.status_flags = 0;
    data_801aa5ce[0] = 0;
    data_801aa6ee[0] = 2;
    player_left.field_01 = 0;
    player_right.field_01 = 0;
    game_state.field_dc &= 0xffc0;
    clear_arrays_and_init_pools_and_sprite_tables();
    func_80136e90();
    func_80136ed0();
    copy_palette_rows_from_backup_bank(1, 0x20, 0);
    copy_palette_rows_from_backup_bank(2, 0x20, 0);
    copy_palette_rows_from_backup_bank(3, 0x20, 0);
    queue_image_upload_job(0, 6);
    queue_image_upload_job(1, 0);
    queue_image_upload_job(2, 1);
    queue_image_upload_job(3, 2);
    func_80157d00(0);
}
