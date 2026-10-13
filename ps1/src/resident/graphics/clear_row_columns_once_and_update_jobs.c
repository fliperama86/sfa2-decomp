/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"

void clear_row_columns_once_and_update_jobs(void) {
    s16 i, j, k;
    u16 *p;

    if (palette_altered_flag == 0) {
        for (i = 0; i < 5; i++) {
            j = 1;
            p = (u16 *)(data_801a27e4 + (i << 10));
            for (; j < 0x10; j++) {
                for (k = 0; k < 0x20; k++) {
                    p[(k << 4) + j] = 0;
                }
            }
        }
        white_fill_active = 1;
        palette_altered_flag = palette_altered_flag + 1;
    }
    queue_five_jobs();
}
