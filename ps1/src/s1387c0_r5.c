/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void select_wide_box_hit_response_or_clear(Object *a0, Object *a1, Box32 *a2) {
    if (a2->box_type == 2 && table_80188ed0[4] < table_80188ed0[0] &&
        table_80188ed0[4] < table_80188ed0[2]) {
        hit_response_by_box_type_4(a0, a1, &a0->wide_boxes[a0->frame->active]);
    } else if ((a0->table_bits & 1) == 0) {
        hit_response_by_box_type_4(a0, a1, &a0->wide_boxes[a0->frame->active]);
    } else {
        mark_hit_and_clear_hit_count(a0, a1);
    }
}

void choose_wide_box_hit_by_table_bit_1(Object *a0, Object *a1, Box32 *unused) {
    if ((a0->table_bits & 2) == 0) {
        hit_response_by_box_type_4(a0, a1, &a0->wide_boxes[a0->frame->active]);
    } else {
        mark_hit_and_clear_hit_count(a0, a1);
    }
}

void dispatch_hit_or_set_pair_states_by_facing(Object *a0, Object *a1, Box32 *a2) {
    if (a0->facing_side == a1->facing_side) {
        dispatch_hit_handler_by_box_type(a0, a1, a2);
    } else if ((u8)is_flag_2_and_box_type_6(a0, a1, a2) != 0 || (u8)is_rising_a_near_in_front_of_b(a0, a1, a2) != 0) {
        set_pair_hit_states_and_play_sound(a0, a1);
    } else {
        dispatch_hit_handler_by_box_type(a0, a1, a2);
    }
}
