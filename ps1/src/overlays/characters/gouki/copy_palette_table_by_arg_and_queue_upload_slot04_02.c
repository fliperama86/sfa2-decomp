/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern u16 data_801c7884_slot04_02[];
extern u16 data_801c7b44_slot04_02[];

void copy_palette_table_by_arg_and_queue_upload_slot04_02(Object *obj, u8 arg) {
    s16 i;

    for (i = 1; i < 16; i++) {
        if (arg == 0) {
            data_801a2bc4[i] = data_801c7884_slot04_02[i];
        } else {
            data_801a2bc4[i] = data_801c7b44_slot04_02[i];
        }
    }
    queue_image_upload_job(0, 6);
}

void copy_next_table_row_and_queue_upload_slot04_02(Object *obj) {
    s16 i;
    int row;

    row = obj->variant_index_2 << 4;
    for (i = 1; i < 16; i++) {
        if (obj->table_index == 0) {
            data_801a2bc4[i] = data_801c7884_slot04_02[row + i];
        } else {
            data_801a2bc4[i] = data_801c7b44_slot04_02[row + i];
        }
    }
    queue_image_upload_job(0, 6);
    obj->variant_index_2++;
}
