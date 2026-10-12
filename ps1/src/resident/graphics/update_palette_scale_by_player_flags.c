/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* The argument is some other struct with a byte at 0x47 and an s16 at 0x66;
   GameState is used as a stand-in with a signed cast. */
void update_palette_scale_by_player_flags(GameState *a) {
    u8 *p = &player_left.field_165;
    int x = *p | p[0x394];

    if (a->pause_flag_2 == 0 && x != 0) {
        if ((x & 0x80) == 0) {
            a->palette_swap_flag = 0xff;
        }
        if ((s16)a->palette_swap_flag == 0) {
            a->palette_swap_flag = 0xff;
            scale_palette_rows_from_backup_bank(0, 4, 0, 0x15);
            scale_palette_rows_from_backup_bank(0, 1, 0, 0x1a);
            scale_palette_rows_from_backup_bank(0, 7, 8, 0);
            if (p[0x394] == 0) {
                scale_palette_rows_from_backup_bank(0, 5, 8, 0xb);
            }
            if (*p == 0) {
                scale_palette_rows_from_backup_bank(0, 5, 8, 0x10);
            }
            scale_palette_rows_from_backup_bank(1, 0x20, 8, 0);
            scale_palette_rows_from_backup_bank(2, 0x20, 8, 0);
            scale_palette_rows_from_backup_bank(3, 0x20, 8, 0);
            scale_palette_rows_from_backup_bank(4, 0x20, 8, 0);
            copy_palette_rows_from_backup_bank(0, 1, 0x1b);
            if (game_state.kind_index == 0xf && game_state.field_226 == 0) {
                if (player_left.kind == 0xf) {
                    scale_palette_rows_from_backup_bank(0, 1, 8, (player_left.image_row + 2) & 0xff);
                } else if (player_right.kind == 0xf) {
                    scale_palette_rows_from_backup_bank(0, 1, 8, (player_right.image_row + 2) & 0xff);
                }
            }
            queue_five_jobs();
        }
    } else if ((s16)a->palette_swap_flag != 0) {
        a->palette_swap_flag = 0;
        copy_palette_rows_from_backup_bank(0, 0x20, 0);
        copy_palette_rows_from_backup_bank(1, 0x20, 0);
        copy_palette_rows_from_backup_bank(2, 0x20, 0);
        copy_palette_rows_from_backup_bank(3, 0x20, 0);
        copy_palette_rows_from_backup_bank(4, 0x20, 0);
        queue_five_jobs();
    }
}
