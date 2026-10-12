/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void restore_rows_from_backup_rows(void) {
    s16 i, j;

    if (data_8018db10) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 0x200; j++) {
                data_801a27e4_rows[i][j] = data_801a27e4_rows[i + 5][j];
            }
        }
        queue_five_jobs();
        data_8018db10 = 0;
    }
}
