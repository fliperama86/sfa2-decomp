/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void update_image_row_copy_with_row_counter(Object *object) {
    advance_sequence_checked(object);
    if (game_state.field_64 != 0) {
        if (*(u8 *)&object->step_flags != 0) return;
        if (object->step_count == 0) game_state.field_dc = 0;
        if (data_8018945c >= 0x10) {
            object->variant_index_2 = 2;
            object->offset_index = 0x17;
            data_8018945c = 0;
        }
        store_image_row_and_queue_job(object, object->variant_index_2, data_8018945c + object->offset_index);
        data_80189458 = data_80189458 + 1;
        if (data_80189458 >= 6) {
            data_8018945c = data_8018945c + 1;
            if (object->variant_index_2 == 2 && data_8018945c >= 5) data_8018945c = 4;
            data_80189458 = 0;
        }
    } else {
        game_state.field_dc = data_80189454;
        object->state_0 = 2;
    }
}
