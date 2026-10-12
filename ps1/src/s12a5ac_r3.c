/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void update_sequence_or_start_special_state(Object *object) {
    if (check_script_condition_by_mode(object)) {
        call_scratch_fn_by_side_and_mode(object);
    } else if ((u8)is_pushed_against_bound_while_moving(object)) {
        start_wall_bounce_sequence(object);
    } else if ((u8)is_once_random_bit_and_script_ready(object)) {
        set_state_3_4_and_sequence(object);
    } else {
        update_sequence(object);
    }
}
