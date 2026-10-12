/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8011f70c(Object *object) {
    if (object->cursor_timer_b != 0) {
        object->field_16a = 1;
        object->cursor_timer_b--;
        object->field_7e = object->field_7e - 1;
        if (object->field_7e & 0x80) {
            object->field_225 = 0;
        }
        object->frame_count = object->frame_count + 1;
        if ((s16)object->frame_count >= 0xae) {
            object->frame_count = 0xad;
        }
    }
}

void count_down_cursor_timer_a_and_set_cursor(Object *object) {
    if (object->cursor_timer_a != 0) {
        object->cursor_timer_a--;
        if (object->side != 0) {
            game_state.cursor = table_80185cf4;
        } else {
            game_state.cursor = table_80185e50;
        }
        object->field_180 = object->field_180 + 1;
        if ((s16)object->field_180 >= 0xae) {
            object->field_180 = 0xad;
        }
    }
}

void advance_sequence_rec(Object *object) {
    SeqRec *rec;
    int v = object->step_timer - 1;
    object->step_timer = v;
    if ((s16)v == 0) {
        rec = (SeqRec *)object->sequence;
        rec++;
        if ((s16)object->step_flags < 0) {
            rec = (SeqRec *)rec->header;
        }
        set_sequence_record_with_default_base(object, rec);
    }
}

void advance_sequence_rec_2(Object *object) {
    SeqRec *rec;
    int v = object->step_timer - 1;
    object->step_timer = v;
    if ((s16)v == 0) {
        rec = (SeqRec *)object->sequence;
        rec++;
        if ((s16)object->step_flags < 0) {
            rec = (SeqRec *)rec->header;
        }
        set_sequence_record_with_default_base_2(object, rec);
    }
}

void advance_sequence_rec_3(Object *object) {
    SeqRec *rec;
    int v = object->step_timer - 1;
    object->step_timer = v;
    if ((s16)v == 0) {
        rec = (SeqRec *)object->sequence;
        rec++;
        if ((s16)object->step_flags < 0) {
            rec = (SeqRec *)rec->header;
        }
        set_sequence_and_copy_data_to_map(object, rec);
    }
}

void set_sequence_from_table(Object *object, SeqRec **table, u8 index) {
    SeqRec **p = table + index;
    set_sequence_record_with_default_base(object, *p);
}

void set_sequence_record_with_default_base(Object *object, SeqRec *rec) {
    set_sequence_and_copy_block(object, rec, data_801aa594);
}

void set_sequence_from_table_2(Object *object, SeqRec **table, u8 index) {
    SeqRec **p = table + index;
    set_sequence_record_with_default_base_2(object, *p);
}

void set_sequence_record_with_default_base_2(Object *object, SeqRec *rec) {
    set_sequence_and_copy_block(object, rec, data_801aa624);
}
