/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"




void func_8013060c(Object *object) {
    push_object_value_to_side_ring(object);
    object->field_262 = 0x10;
    if (object->field_267 == 0) {
        object->field_25c = 0;
        if (object->side == 0) scratch_fn_left(object);
        else scratch_fn_right(object);
    }
}

void set_sequence_by_side(Object *object, int arg) {
    u16 index = arg;
    if (object->side == 0) object->sequence = seqs_60_left[index];
    else object->sequence = seqs_110_right[index];
    object->step_timer = object->sequence->duration;
    object->step_flags = object->sequence->flags;
    object->sequence_changed_flag = 1;
    object->frame = object->frames + object->sequence->frame_index;
    object->frame_sound_index = object->frame->sequence_param;
}

void set_sequence_entry(Object *object, SequenceStep *entry) {
    object->sequence = entry;
    object->step_timer = entry->duration;
    object->step_flags = object->sequence->flags;
    object->sequence_changed_flag = 1;
    if (object->mode_id == 0 || object->mode_id == 8) {
        object->frame = object->frames + object->sequence->frame_index;
        object->frame_sound_index = object->frame->sequence_param;
    } else {
        object->frame_sound_index = 0;
    }
}

void set_sequence_from_table_5(Object *object, int arg, SequenceStep **table) {
    s16 index = arg;
    object->sequence = table[index];
    object->step_timer = object->sequence->duration;
    object->step_flags = object->sequence->flags;
    object->sequence_changed_flag = 1;
    if (object->mode_id == 0 || object->mode_id == 8) {
        object->frame = object->frames + object->sequence->frame_index;
        object->frame_sound_index = object->frame->sequence_param;
    } else {
        object->frame_sound_index = 0;
    }
}

void set_sequence_by_side_and_mode(Object *object, int arg) {
    u16 index = arg;
    if (object->mode_flag_2 == 0) {
        if (object->side == 0) object->sequence = sequences_left[index];
        else object->sequence = sequences_right[index];
    } else {
        if (object->side == 0) object->sequence = seqs_9c_left[index];
        else object->sequence = seqs_14c_right[index];
    }
    object->step_timer = object->sequence->duration;
    object->step_flags = object->sequence->flags;
    object->sequence_changed_flag = 1;
    object->frame = object->frames + object->sequence->frame_index;
    object->frame_sound_index = object->frame->sequence_param;
    advance_sequence_if_state_and_not_held(object);
}

void set_sequence_by_side_and_mode_2(Object *object, u16 index) {
    SequenceStep *entry;
    if (object->field_295 != 0) {
        if (object->side == 0) entry = seqs_6c_left[index];
        else entry = seqs_11c_right[index];
    } else {
        if (object->side == 0) entry = seqs_68_left[index];
        else entry = seqs_118_right[index];
    }
    if (object->sequence != entry) object->sequence_changed_flag = 1;
    object->sequence = entry;
    object->step_timer = entry->duration;
    object->step_flags = object->sequence->flags;
    object->frame = object->frames + object->sequence->frame_index;
    object->frame_sound_index = object->frame->sequence_param;
}
