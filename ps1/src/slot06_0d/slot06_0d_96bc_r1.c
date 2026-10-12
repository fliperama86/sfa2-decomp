/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801ea520_slot06_0d[];
extern SequenceStep *data_801eb1b0_slot06_0d[];

void advance_state_1_on_pause_flag_or_advance_slot06_0d(Object *obj);
void call_advance_sequence_checked_slot06_0d(Object *obj);
void advance_cursor_record_countdown_slot06_0d(Slot06Cursor *cur);
void load_palette_record_by_index_slot06_0d(Slot06Pal *pal);
void set_palette_record_and_copy_data_slot06_0d(Slot06Pal *pal, SeqRec *rec);

void call_handler_by_state_0_slot06_0d(Object *obj) {
    data_801ea520_slot06_0d[obj->state_0](obj);
}

void init_object_and_start_sequence_by_count_index_slot06_0d(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    int i = 1;
    o->spawn_mode = 4;
    o->pair_value = 0xd;
    obj->init_param_f = 0x1f0;
    o->state_0 = o->state_0 + 1;
    o->field_01 = 0;
    o->image_row = 0;
    o->field_0c = 0;
    o->field_0a = i;
    o->field_0f = i;
    obj->init_param_g = 0x80;
    o->state_0 = i;
    load_palette_record_by_index_slot06_0d((Slot06Pal *)((u8 *)o + 0x5c));
    i = game_state.count_index ? 0xa : 3;
    set_sequence_entry(o, data_801eb1b0_slot06_0d[i]);
}

void update_object_by_state_1_and_cursor_slot06_0d(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            if (obj->state_1 == 0) {
                advance_state_1_on_pause_flag_or_advance_slot06_0d(obj);
            } else if (obj->state_1 == 1) {
                call_advance_sequence_checked_slot06_0d(obj);
            }
            advance_cursor_record_countdown_slot06_0d((Slot06Cursor *)((u8 *)obj + 0x5c));
        }
    }
    set_flag_and_push_if_mode_8(obj);
}

void advance_state_1_on_pause_flag_or_advance_slot06_0d(Object *obj) {
    int i;
    if (game_state.pause_flag_2 != 0 && game_state.countdown_timer == 0) {
        obj->state_1 = obj->state_1 + 1;
        i = game_state.focus_object->kind != 0xd ? 0xb : 0xc;
        set_sequence_entry(obj, data_801eb1b0_slot06_0d[i]);
    } else {
        advance_sequence_checked(obj);
    }
}

void call_advance_sequence_checked_slot06_0d(Object *obj) {
    advance_sequence_checked(obj);
}

void release_object_record_slot06_0d(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void advance_cursor_record_countdown_slot06_0d(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->remaining_count - 1;
    cur->remaining_count = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->step_word < 0) {
            rec = (SeqRec *)rec->header;
        }
        set_palette_record_and_copy_data_slot06_0d((Slot06Pal *)cur, rec);
    }
}

void load_palette_record_by_index_slot06_0d(Slot06Pal *pal) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    set_palette_record_and_copy_data_slot06_0d(pal, (SeqRec *)table_8017c910[pal->seq_rec_index]);
}
