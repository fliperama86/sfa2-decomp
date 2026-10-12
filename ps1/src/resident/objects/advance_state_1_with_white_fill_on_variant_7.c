/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void advance_state_1_with_white_fill_on_variant_7(Object *object) {
    if (object->variant_index_2 == 7) {
        if (object->step_count++ == 2) {
            object->step_count = 0;
            object->field_01 = 0;
            fill_row_columns_white_once();
            object->state_1 = object->state_1 + 1;
            if (object->field_a3 == 0) play_sound_at(0, 0, 0x206);
        }
    } else {
        object->state_1 = object->state_1 + 1;
    }
}

void advance_state_1_with_row_restore_on_variant_7(Object *object) {
    object->field_0c = 0;
    if (object->variant_index_2 == 7) {
        if (object->step_count++ == 2) {
            object->field_01 = 1;
            restore_rows_from_backup_rows();
            if (object->field_a3 == 0) {
                object->state_1--;
                object->step_count = 1;
                object->field_a3 = 1;
                data_8018f598 = 1;
            } else {
                object->step_count = 0;
                object->field_a3 = 0;
                object->state_1 = object->state_1 + 1;
            }
        }
    } else {
        object->state_1 = object->state_1 + 1;
    }
}
