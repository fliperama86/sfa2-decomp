/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void set_box_tables_from_health_index_slot04_00(Object *object);

void advance_state_2_set_box_tables_and_sequence_slot04_00(Object *object) {
    object->state_2 = object->state_2 + 1;
    set_box_tables_from_health_index_slot04_00(object);
    object->countdown = 2;
    advance_sequence_checked(object);
}
