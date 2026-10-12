/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void update_sequence_or_set_state_3_to_1(Object *object) {
    if (check_script_condition_by_mode(object)) {
        call_scratch_fn_by_side_and_mode(object);
    } else {
        if (!check_enemy_unit_or_other_in_range(object)) {
            object->state_3 = 1;
        }
        update_sequence(object);
    }
}
