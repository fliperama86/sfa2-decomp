/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801428e4(Object *object) {
    if (object->mode_flag_2 != 0) {
        update_meter_by_direction_index(object);
    } else {
        add_to_meter_clamped(object, table_8017ad54[object->field_255 >> 1]);
    }
}

void func_80142940(Object *object) {
    object->direction_index = 4;
    object->field_255 = 4;
    update_meter_by_direction_index(object);
}
