/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801ea968_slot06_10[];
extern ObjectFn data_801ea978_slot06_10[];
extern void (*data_801ea988_slot06_10[])(Object *, Object *);
extern SeqRec data_801ea944_slot06_10[];
extern SequenceStep *data_801ebbc8_slot06_10[];

void advance_record_cursor_slot06_10(Slot06Cursor *cur);
void init_cursor_from_record_header_slot06_10(Slot06Cursor *cur, SeqRec *rec);
void init_sequence_cursor_from_record_2_slot06_10(SeqRec *rec, Slot06Cursor *cur);
void advance_record_cursor_2_slot06_10(Slot06Cursor *cur);
void init_cursor_from_record_header_2_slot06_10(Slot06Cursor *cur, SeqRec *rec);
void set_state_1_3_if_pause_flag_2_slot06_10(Object *obj, int unused, u8 index);
void start_sequence_1_when_other_crosses_x_slot06_10(Object *obj, Object *other);
void start_sequence_2_when_other_crosses_back_slot06_10(Object *obj, Object *other);
void count_up_then_start_sequence_3_slot06_10(Object *obj, Object *other);

void advance_record_cursor_slot06_10(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->remaining_count - 1;
    cur->remaining_count = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->step_word < 0) {
            rec = (SeqRec *)rec->header;
        }
        init_cursor_from_record_header_slot06_10(cur, rec);
    }
}

void init_cursor_from_record_header_slot06_10(Slot06Cursor *cur, SeqRec *rec) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->remaining_count = new_var;
    cur->step_word = w;
    store_rect_entries_and_queue_kinds(rec);
}

void call_handler_by_state_0_2_slot06_10(Object *obj) {
    data_801ea968_slot06_10[obj->state_0](obj);
}

void advance_state_0_and_init_cursor_slot06_10(Object *obj) {
    obj->field_01 = 0;
    obj->state_0 = obj->state_0 + 1;
    init_sequence_cursor_from_record_2_slot06_10(data_801ea944_slot06_10, (Slot06Cursor *)((u8 *)obj + 0x28));
}

void init_sequence_cursor_from_record_2_slot06_10(SeqRec *rec, Slot06Cursor *cur) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->remaining_count = new_var;
    cur->step_word = w;
}

void advance_object_record_cursor_unless_paused_slot06_10(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            advance_record_cursor_2_slot06_10((Slot06Cursor *)((u8 *)obj + 0x28));
        }
    }
}

void release_object_record_2_slot06_10(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void advance_record_cursor_2_slot06_10(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->remaining_count - 1;
    cur->remaining_count = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->step_word < 0) {
            rec = (SeqRec *)rec->header;
        }
        init_cursor_from_record_header_2_slot06_10(cur, rec);
    }
}

void init_cursor_from_record_header_2_slot06_10(Slot06Cursor *cur, SeqRec *rec) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->remaining_count = new_var;
    cur->step_word = w;
    store_rect_entries_and_queue_kinds(rec);
}

void call_handler_by_state_0_3_slot06_10(Object *obj) {
    data_801ea978_slot06_10[obj->state_0](obj);
}

void init_object_with_countdown_sequence_0_slot06_10(Object *obj) {
    obj->field_0f = 1;
    obj->state_0 = 1;
    obj->base_x = 0x340;
    obj->base_y = 0x100;
    obj->spawn_x = 0x70;
    obj->spawn_y = 0x1e0;
    obj->state_1 = 0;
    obj->countdown = 0;
    obj->image_row = 0;
    obj->spawn_mode = 4;
    obj->pos_y = obj->pos_y + 0x70;
    set_sequence_entry(obj, data_801ebbc8_slot06_10[0]);
}

void call_state_1_handler_with_player_a_slot06_10(Object *obj) {
    Object *other;
    if ((game_state.field_65 | game_state.field_74) == 0) {
        other = &player_left;
        if (obj->table_index != 0) {
            other = other + 1;
        }
        data_801ea988_slot06_10[obj->state_1](obj, other);
    }
    clear_or_set_flag_by_pos_x_limit(obj);
}

void update_sequence_or_check_other_crossing_x_slot06_10(Object *obj, Object *other) {
    if ((s16)obj->step_flags & 0x8000) {
        set_state_1_3_if_pause_flag_2_slot06_10(obj, (int)other, 0);
        start_sequence_1_when_other_crosses_x_slot06_10(obj, other);
    } else {
        advance_sequence_checked(obj);
    }
}

void update_sequence_or_check_countdown_and_crossing_slot06_10(Object *obj, Object *other) {
    if ((s16)obj->step_flags & 0x8000) {
        set_state_1_3_if_pause_flag_2_slot06_10(obj, (int)other, 3);
        count_up_then_start_sequence_3_slot06_10(obj, other);
        start_sequence_2_when_other_crosses_back_slot06_10(obj, other);
    } else {
        advance_sequence_checked(obj);
    }
}
