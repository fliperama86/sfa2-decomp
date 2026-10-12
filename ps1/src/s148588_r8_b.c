/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void call_table_function_by_state_0_9(Block172 *block) {
    Object *object = (Object *)block;
    table_8017cf00[object->state_0](object);
}

void advance_state_0_call_table_handler(Object *object) {
    object->state_0 = object->state_0 + 1;
    table_8017cf10[object->table_index](object);
}

void init_depth_six_shift_x_and_set_sequence(Object *object) {
    object->ot_depth = 6;
    shift_pos_x_by_parent_kind_table(object);
    set_sequence_from_table_5(object, 0x25, seqs_8017c7f8);
}

void init_depth_eight_shift_x_and_set_sequence(Object *object) {
    object->ot_depth = 8;
    shift_pos_x_by_parent_kind_table(object);
    set_sequence_from_table_5(object, 0x26, seqs_8017c7f8);
}

void init_depth_six_play_parent_sound_set_sequence(Object *object) {
    Object *other = object->parent;
    object->ot_depth = 6;
    play_sound_at(other, other->side, 0x326);
    set_sequence_from_table_5(object, 0x27, seqs_8017c7f8);
}

void start_random_sequence_at_random_offset(Object *object) {
    int dx;
    int dy;
    object->ot_depth = 6;
    dx = (get_random_byte() & 0x1f) - 0x10;
    object->pos_x = object->pos_x + dx;
    dy = (get_random_byte() & 0x1f) + 0x10;
    object->pos_y = object->pos_y - dy;
    set_sequence_from_table_5(object, (get_random_byte() & 7) | 0x28, seqs_8017c7f8);
}

void call_handler_by_table_index_0(Object *object) {
    table_8017cf20[object->table_index](object);
    set_flag_and_push_if_mode_8(object);
}

void advance_state_0_if_parent_flag_clear(Object *object) {
    ref_other.p = object->parent;
    if (ref_other.p->field_165 == 0) {
        object->state_0 = object->state_0 + 1;
    }
    advance_sequence_checked(object);
}

void call_advance_state_0_if_parent_flag_clear(Object *object) {
    advance_state_0_if_parent_flag_clear(object);
}

void play_parent_sounds_on_step_flags(Object *object) {
    ref_other.p = object->parent;
    if ((s16)object->step_flags < 0) {
        object->state_0 = object->state_0 + 1;
        play_sound_at(ref_other.p, ref_other.p->side, 0x328);
    }
    /* Byte load of the low half of step_flags. */
    if (*(u8 *)&object->step_flags != 0) {
        play_sound_at(ref_other.p, ref_other.p->side, 0x327);
    }
    advance_sequence_checked(object);
}

void advance_sequence_next_state_on_flags(Object *object) {
    if ((s16)object->step_flags < 0) {
        object->state_0 = object->state_0 + 1;
    }
    advance_sequence_checked(object);
}
