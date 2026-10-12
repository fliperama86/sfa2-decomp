/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void select_box_data_pointer_by_health_slot04_0b(Object *object);

void advance_state_2_select_boxes_by_health_slot04_0b(Object *object) {
    object->state_2 = object->state_2 + 1;
    select_box_data_pointer_by_health_slot04_0b(object);
    object->countdown = 2;
    advance_sequence_checked(object);
}
