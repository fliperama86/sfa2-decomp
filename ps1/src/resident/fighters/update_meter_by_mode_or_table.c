/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


void update_meter_by_mode_or_table(Object *object) {
    if (object->mode_flag_2 != 0) {
        update_meter_by_direction_index(object);
    } else {
        add_to_meter_clamped(object, table_8017ad54[object->field_255 >> 1]);
    }
}

void set_direction_index_four_then_update_meter(Object *object) {
    object->direction_index = 4;
    object->field_255 = 4;
    update_meter_by_direction_index(object);
}
