/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



int select_inputs_from_threshold_mask(Object *object, Box32 *box, s16 elapsed) {
    s16 mask;
    s16 i, shift;
    mask = build_mask_of_reached_thresholds(object, elapsed);
    mask = clear_mask_bits_below_target_position(object, mask);
    for (i = 11; i != -1; --i) {
        if (game_state.other_object->input_dir_0) shift = shift_b[i];
        else shift = shift_a[i];
        mask = mask >> shift;
        if (mask) {
            object->input_dir_0 = quad_table[shift * 4 + 0];
            object->input_dir_second = quad_table[shift * 4 + 1];
            object->direction_index = quad_table[shift * 4 + 2];
            object->attack_param_d = quad_table[shift * 4 + 3];
            return 1;
        }
    }
}

s16 build_mask_of_reached_thresholds(Object *object, s16 value) {
    s16 mask;
    s16 i, limit;
    i = 0;
    if (object->side) game_state.pair_ptr = (Pair *)metrics_right;
    else game_state.pair_ptr = (Pair *)metrics_left;
    do {
        limit = game_state.pair_ptr->first;
        game_state.pair_ptr++;
        if (!(value < limit)) mask |= 1 << i;
        i++;
    } while (i < 12);
    return mask;
}
