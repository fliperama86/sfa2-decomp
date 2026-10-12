/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

void spawn_two_pool_objects_a9_and_ab_slot27(void) {
    Object *o;
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0xa9;
        o->table_index = 0;
    }
    o = (Object *)pop_block_from_pool();
    if (o != 0) {
        o->active_count = 1;
        o->field_02 = 0xab;
        o->table_index = 0;
    }
}

void fill_rows_white_and_queue_jobs_slot27(void) {
    int i;
    int j;
    u16 *row;
    if (data_8018db10 == 0) {
        for (i = 0; i < 5; i++) {
            row = data_801a27e4_rows[i];
            for (j = 0x1ff; j >= 0; j--) {
                row[j] = 0x7fff;
            }
        }
        queue_image_upload_job(0, 0);
        queue_image_upload_job(1, 1);
        queue_image_upload_job(2, 2);
        queue_image_upload_job(3, 3);
        queue_image_upload_job(4, 7);
        data_8018db10++;
    }
}
