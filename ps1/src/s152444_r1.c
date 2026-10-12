/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void call_handler_by_variant_index(Object *object) {
    table_80180284[object->variant_index >> 1](object);
}

void call_state_1_table_then_push_if_mode_8(Object *object) {
    table_80180294[object->state_1](object);
    set_flag_and_push_if_mode_8(object);
}

void start_random_sequence_by_variant(Object *object) {
    object->state_1++;
    set_random_sequence_from_variant(object, object->variant_index);
    set_position_from_parent_table(object);
}

void follow_parent_and_start_sequence_when_clear(Object *object) {
    if (is_linked_object_field_clear(object)) {
        object->state_1++;
        set_sequence_from_table_5(object, (object->variant_index * 2 + 0x33) & 0xff, table_8017c7f8);
    }
    set_position_from_parent_table(object);
    advance_sequence_checked(object);
}

void advance_sequence_or_next_state(Object *object) {
    if ((s16)object->step_flags >= 0) {
        advance_sequence_checked(object);
    } else {
        object->state_0++;
    }
}

void call_state_1_table_handler_wrapper(Object *object) {
    call_state_1_table_then_push_if_mode_8(object);
}

void call_state_1_handler_unless_released(Object *object) {
    if (is_linked_object_field_clear(object)) {
        object->state_0++;
    } else {
        table_801802a0[object->state_1](object);
        set_flag_and_push_if_mode_8(object);
    }
}

void start_sequence_from_countdown_table(Object *object) {
    object->state_1++;
    set_sequence_from_table_5(object, (table_801802a8[(s16)object->countdown] + (object->table_index >> 2)) & 0xff, table_8017c7f8);
    set_position_from_parent_offset_table(object);
}

void advance_sequence_and_follow_parent_offset(Object *object) {
    advance_sequence_checked(object);
    set_position_from_parent_offset_table(object);
}
