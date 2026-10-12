/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void copy_palette_rows_from_backup_bank(u8 bank, u8 count, u8 row) {
    u8 i, j;
    for (i = 0; i < count; i++, row++)
        for (j = 0; j < 16; j++)
            data_801a27e4_rows[bank][row * 16 + j] =
                data_801a27e4_rows[bank + 5][row * 16 + j];
}
