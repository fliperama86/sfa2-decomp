/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern void (*data_800267a4_slot27[])(Object *);
extern void (*data_800267b4_slot27[])(Object *);

void clear_rows_and_queue_jobs_slot27(void) {
    int set;
    int i;
    int j;

    if (palette_altered_flag == 0) {
        for (set = 0; set < 5; set++) {
            for (i = 0; i < 0x10; i++) {
                for (j = 0; j < 0x20; j++) {
                    data_801a27e4_rows[set][j * 0x10 + i] = 0;
                }
            }
        }
        white_fill_active = 1;
        palette_altered_flag = palette_altered_flag + 1;
    }
    queue_image_upload_job(0, 0);
    queue_image_upload_job(1, 1);
    queue_image_upload_job(2, 2);
    queue_image_upload_job(3, 3);
    queue_image_upload_job(4, 7);
}

void call_handler_by_state_0_slot27(Object *object) {
    data_800267a4_slot27[object->state_0](object);
}

void call_state_1_handler_from_slot27_table_a_slot27(Object *object) {
    data_800267b4_slot27[object->state_1](object);
}
