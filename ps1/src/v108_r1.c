/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

/* Exact. The cd == 0 arm carries its own copy of the call (the compiler merged only the jal). */
/* Historical note, from before this unit was exact or about a function that is no longer in this unit: Residual: not exact, same size, 7 slots. The constant 1 now matches (active_flag_3 = 1
 * is stored next to the kind test). Only the delay slots and targets of the early-out
 * branches differ: the original sends the kind, field_163 and hit_flag tests to the
 * move before the call (nop slots) and fills the slot of the health test with it. */
void choose_recovery_sequence_or_state(Object *object) {
    if (object->mode_flag_2 == 0) {
        set_sequence_by_side_and_mode_2(object, 0x14);
        return;
    }
    if ((object->kind != 6 && object->kind != 0xf) ||
        object->field_163 != 0 || object->hit_flag != 0 || (s16)object->health < 0 ||
        ((game_state.config->mode_flag_b | game_state.config->mode_flag_c) | game_state.config->mode_flag_a) != 0 ||
        is_other_object_state_0_and_1_six_one(object) || !test_random_bit_in_table(object, data_8017d898)) {
        set_sequence_by_side_and_mode_2(object, 0x14);
    } else {
        object->state_0 = 1;
        object->state_2 = 7;
        object->direction_index = 2;
        object->field_15a = 6;
        object->state_1 = 0;
        object->state_3 = 0;
        object->once_flag_2 = 0;
        object->done_flag = 0;
        object->flag_25a_set = 0;
        object->active_flag_3 = 1;
        if (object->kind == 0xf) {
            object->active_flag_3 = 0;
        }
    }
}
