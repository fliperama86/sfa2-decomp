/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void advance_state_1_on_variant_index_set_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->variant_index = 0;
        obj->state_1++;
    }
}

void move_left_to_x_c0_then_advance_state_1_slot28(Object *obj) {
    *(s32 *)&obj->pos_x_frac += 0xffff8000;
    if (obj->pos_x <= 0xc0) {
        obj->pos_x = 0xc0;
        obj->state_1++;
    }
}

void start_sequence_7_on_variant_index_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->variant_index = 0;
        obj->state_1++;
        set_sequence_from_table_5(obj, 7, (SequenceStep **)obj->box_tables);
    }
}

void start_sequence_8_when_far_from_parent_slot28(Object *obj) {
    if (obj->pos_x - obj->parent->pos_x >= 0x30) {
        obj->state_1++;
        set_sequence_from_table_5(obj, 8, (SequenceStep **)obj->box_tables);
    }
}
