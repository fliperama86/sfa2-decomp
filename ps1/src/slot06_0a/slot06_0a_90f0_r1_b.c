/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ed400_slot06_0a[];
extern ObjectFn data_801ed460_slot06_0a[];
extern ObjectFn data_801ed470_slot06_0a[];
extern ObjectFn data_801ed480_slot06_0a[];

void set_sequence_8_on_player_state_2_eight_slot06_0a(Object *obj);
void start_sequence_when_target_near_x_slot06_0a(Object *obj);
void call_handler_by_state_1_then_offset_slot06_0a(Object *obj);
void start_sequence_when_target_far_x_slot06_0a(Object *obj);

void set_position_from_scaled_offsets_half_slot06_0a(Object *obj, int a, int b) {
    int t = a >> 1;
    int u = b >> 1;
    a = (a >> 6) + t;
    b = (b >> 4) + u;
    a += obj->vel_x;
    b += obj->vel_y;
    *(s32 *)&obj->pos_x_frac = a;
    *(s32 *)&obj->pos_y_frac = 0xf80000 - b;
}

void set_position_from_inputs_x_reduced_slot06_0a(Object *obj, int a, int b) {
    a = a - (a >> 4);
    a += obj->vel_x;
    b += obj->vel_y;
    *(s32 *)&obj->pos_x_frac = a;
    *(s32 *)&obj->pos_y_frac = 0xf80000 - b;
}

void release_object_record_slot06_0a(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_2_slot06_0a(Object *obj) {
    data_801ed460_slot06_0a[obj->state_0](obj);
}

void init_object_find_kind_a_player_target_slot06_0a(Object *obj) {
    int i = 7;
    Object *a = &player_left;
    Object *b = a + 1;

    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->field_0c = 0;
    if (obj->table_index != 0) {
        i = 0xd;
        a = b;
        b = a - 1;
    }
    if (a->kind == 0xa) {
        ((ObjectWords *)obj)->word_4c = (s32)a;
    } else if (b->kind == 0xa) {
        ((ObjectWords *)obj)->word_4c = (s32)b;
    } else {
        ((ObjectWords *)obj)->word_4c = 0;
    }
    set_sequence_entry(obj, data_801ed400_slot06_0a[i]);
}

void call_state_1_handler_unless_paused_slot06_0a(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        if (obj->table_index != 0) {
            call_handler_by_state_1_then_offset_slot06_0a(obj);
            return;
        }
        data_801ed470_slot06_0a[obj->state_1](obj);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void check_near_target_and_player_state_then_advance_slot06_0a(Object *obj) {
    start_sequence_when_target_near_x_slot06_0a(obj);
    set_sequence_8_on_player_state_2_eight_slot06_0a(obj);
    advance_sequence_checked(obj);
}

void on_sequence_end_set_state_1_two_seq_5_slot06_0a(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1 = 2;
        set_sequence_entry(obj, data_801ed400_slot06_0a[5]);
    } else {
        advance_sequence_checked(obj);
    }
}

void check_far_distance_on_flag_then_advance_slot06_0a(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        start_sequence_when_target_far_x_slot06_0a(obj);
    }
    advance_sequence_checked(obj);
}

void on_sequence_end_set_state_1_zero_seq_7_slot06_0a(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1 = 0;
        set_sequence_entry(obj, data_801ed400_slot06_0a[7]);
    } else {
        advance_sequence_checked(obj);
    }
}

void call_handler_by_state_1_then_offset_slot06_0a(Object *obj) {
    data_801ed480_slot06_0a[obj->state_1](obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void start_sequence_if_target_near_then_advance_slot06_0a(Object *obj) {
    start_sequence_when_target_near_x_slot06_0a(obj);
    advance_sequence_checked(obj);
}

void switch_to_sequence_10_at_flag_end_slot06_0a(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1 = 2;
        set_sequence_entry(obj, data_801ed400_slot06_0a[0xa]);
    } else {
        advance_sequence_checked(obj);
    }
}

void switch_to_sequence_13_at_flag_end_slot06_0a(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        obj->state_1 = 0;
        set_sequence_entry(obj, data_801ed400_slot06_0a[0xd]);
    } else {
        advance_sequence_checked(obj);
    }
}

void set_sequence_8_on_player_state_2_eight_slot06_0a(Object *obj) {
    if (player_left.state_2 == 8 || player_right.state_2 == 8) {
        obj->state_1 = 3;
        set_sequence_entry(obj, data_801ed400_slot06_0a[8]);
    }
}
