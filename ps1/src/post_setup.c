/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void clamp_pos_x_to_bounds_and_push_other(Object *object) {
    int limit;
    int dist;
    s16 x;
    int d;
    int span;

    object->bound_push_flag = 0;
    ref_other.p = object->other;
    limit = 0x100;
    if ((s16)object->health >= 0 && (s16)ref_other.p->health >= 0) limit = 0x150;
    x = object->pos_x;
    dist = ref_other.p->pos_x - x;
    if (dist < 0) dist = -dist;
    span = (s16)object->half_width;
    d = x - span - (s16)box_margin[0];
    if (d <= 0) {
        object->pos_x = x - d;
        object->bound_push_flag = 1;
        if (*(u16 *)&object->state_0 == 0x101 && object->field_241 == 0 && object->active_state == 0 && dist < limit) goto push;
    } else {
        d += span * 2;
        d -= 0x180;
        if (d >= 0) {
            object->pos_x = x - d;
            object->bound_push_flag = 2;
            if (*(u16 *)&object->state_0 == 0x101 && object->field_241 == 0 && object->active_state == 0 && dist < limit) {
push:
                if (object->flag_166_set == 0 && ref_other.p->active_state == 0) {
                    ref_other.p->pos_x = ref_other.p->pos_x - d;
                }
            }
        }
    }
}

void push_both_objects_inside_bounds(Object *object) {
    s16 d;
    int span;
    u16 x;

    ref_other.p = object->other;
    ref_first.p = object->other;
    ref_second.p = object;
    ref_other.p->bound_push_flag = 0;
    if (!(ref_other.p->pos_x < object->pos_x)) {
        ref_first.p = object;
        ref_second.p = ref_other.p;
    }
    span = ref_first.p->half_width;
    x = ref_first.p->pos_x;
    d = x - span - box_margin[0];
    if (d < 0) {
        object->pos_x = object->pos_x - d;
        ref_other.p->pos_x = ref_other.p->pos_x - d;
        ref_other.p->bound_push_flag = 1;
    } else {
        x = ref_second.p->pos_x;
        span = ref_second.p->half_width;
        d = x - box_margin[0] + span - 0x180;
        if (d >= 0) {
            object->pos_x = object->pos_x - d;
            ref_other.p->pos_x = ref_other.p->pos_x - d;
            ref_other.p->bound_push_flag = 2;
        }
    }
}

void set_sequence_and_tables_from_direction(Object *object) {
    int half;
    s16 index;
    int bonus;

    reset_direction_and_action_fields(object);
    half = object->direction_index >> 1;
    if (object->kind != 9) {
        add_to_meter_clamped(object, half);
    } else {
        object->field_225 = 1;
        if (object->counter == 0) {
            object->field_225 = 0;
            add_to_meter_clamped(object, half);
        }
    }
    index = 0;
    if (object->input_dir_0 != 0) {
        index = 6;
        object->field_157 = object->field_157 + 1;
    }
    bonus = 0;
    if (object->input_dir_second != 0) bonus = 3;
    index = index + half;
    index = index + bonus;
    set_sequence_by_side_and_mode(object, (u16)index);
    object->kind_table_value = kind_table_248[object->kind * 16 + index];
    object->countdown_5 = kind_table_29b[object->kind * 16 + index];
}

void reset_direction_and_action_fields(Object *object) {
    object->field_278 = 0;
    object->dir_match_flag = 0;
    object->field_157 = 0;
    object->active_flag = 0;
    object->field_16a = 0;
    object->dir_first = 0xff;
    object->dir_second = 0xff;
    reset_hold_flags_and_other_hit_count(object);
}

void update_sequence(Object *object) {
    advance_sequence(object);
    advance_sequence_if_state_and_not_held(object);
}

void advance_sequence(Object *object) {
    SequenceStep *next;

    if (--*(s16 *)&object->step_timer == 0) {
        if ((s16)object->step_flags < 0) {
            next = object->sequence + object->sequence->loop_offset;
            if (next != object->sequence) object->sequence_changed_flag = 1;
            object->sequence = object->sequence + object->sequence->loop_offset;
        } else {
            next = object->sequence + 1;
            if (next != object->sequence) object->sequence_changed_flag = 1;
            object->sequence = object->sequence + 1;
        }
        object->step_timer = object->sequence->duration;
        object->step_flags = object->sequence->flags;
        object->frame = object->frames + object->sequence->frame_index;
        object->frame_sound_index = object->frame->sequence_param;
    }
}

void advance_sequence_if_state_and_not_held(Object *object) {
    u32 w;

    if (object->field_7e != 0) {
        w = *(u32 *)&object->state_0;
        if ((u16)w == 1) {
            if ((w & 0xffff0000) == 0x03030000 || object->state_2 == 5) {
                if (object->sequence_hold_flag == 0) advance_sequence(object);
            }
        }
    }
}

void advance_sequence_checked(Object *object) {
    SequenceStep *next;

    if (--*(s16 *)&object->step_timer == 0) {
        if ((s16)object->step_flags < 0) {
            next = object->sequence + object->sequence->loop_offset;
            if (next != object->sequence) object->sequence_changed_flag = 1;
            object->sequence = object->sequence + object->sequence->loop_offset;
        } else {
            next = object->sequence + 1;
            if (next != object->sequence) object->sequence_changed_flag = 1;
            object->sequence = object->sequence + 1;
        }
        object->step_timer = object->sequence->duration;
        object->step_flags = object->sequence->flags;
        if (object->mode_id == 0 || object->mode_id == 8) {
            object->frame = object->frames + object->sequence->frame_index;
            object->frame_sound_index = object->frame->sequence_param;
        } else {
            object->frame_sound_index = 0;
        }
    }
}
