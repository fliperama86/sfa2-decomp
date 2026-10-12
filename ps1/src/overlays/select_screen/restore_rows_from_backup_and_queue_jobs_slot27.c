/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void restore_rows_from_backup_and_queue_jobs_slot27(void) {
    int i;
    int j;
    if (data_8018db10 != 0) {
        for (i = 0; i < 5; i++) {
            for (j = 0; j < 0x200; j++) {
                data_801a27e4_rows[i][j] = data_801a27e4_rows[i + 5][j];
            }
        }
        queue_image_upload_job(0, 0);
        queue_image_upload_job(1, 1);
        queue_image_upload_job(2, 2);
        queue_image_upload_job(3, 3);
        queue_image_upload_job(4, 7);
        data_8018db10 = 0;
    }
}
