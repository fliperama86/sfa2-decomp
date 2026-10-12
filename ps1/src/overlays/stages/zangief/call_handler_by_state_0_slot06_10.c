/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectFn data_801ea958_slot06_10[];
extern SeqRec data_801ea868_slot06_10[];
extern SeqRec data_801ea8a0_slot06_10[];
extern SeqRec data_801ea8d8_slot06_10[];

void init_sequence_cursor_from_record_slot06_10(SeqRec *rec, Slot06Cursor *cur);
void advance_palette_record_countdown_slot06_10(Slot06Pal *pal);
void load_palette_record_by_index_slot06_10(Slot06Pal *pal);
void set_palette_record_and_copy_data_slot06_10(Slot06Pal *pal, SeqRec *rec);
void advance_record_cursor_slot06_10(Slot06Cursor *cur);

void call_handler_by_state_0_slot06_10(Object *obj) {
    data_801ea958_slot06_10[obj->state_0](obj);
}

void init_object_params_palettes_and_cursors_slot06_10(Object *o) {
    Slot06Obj *obj = (Slot06Obj *)o;
    obj->init_param_b = 0x10;
    obj->table_base_index = 0x11;
    obj->init_param_e = 0x12;
    obj->init_param_a = 0x2e0;
    obj->sequence_index = 0x200;
    obj->size_param = 0x180;
    obj->field_01 = 0;
    obj->offset_b = 0x50;
    obj->init_param_c = 0x50;
    obj->init_param_d = 0x50;
    obj->step_count = obj->step_count + 1;
    load_palette_record_by_index_slot06_10((Slot06Pal *)((u8 *)obj + 0x30));
    load_palette_record_by_index_slot06_10((Slot06Pal *)((u8 *)obj + 0x40));
    load_palette_record_by_index_slot06_10((Slot06Pal *)((u8 *)obj + 0x50));
    init_sequence_cursor_from_record_slot06_10(data_801ea868_slot06_10, (Slot06Cursor *)((u8 *)obj + 0x28));
    init_sequence_cursor_from_record_slot06_10(data_801ea8a0_slot06_10, (Slot06Cursor *)((u8 *)obj + 0x6c));
    init_sequence_cursor_from_record_slot06_10(data_801ea8d8_slot06_10, (Slot06Cursor *)((u8 *)obj + 0x84));
}

void init_sequence_cursor_from_record_slot06_10(SeqRec *rec, Slot06Cursor *cur) {
    int w;
    int new_var;
    cur->rec = rec;
    new_var = rec->header;
    w = new_var;
    new_var = w >> 16;
    cur->remaining_count = new_var;
    cur->step_word = w;
}

void update_palette_and_cursor_records_slot06_10(Object *obj) {
    if (game_state.field_65 == 0) {
        if (game_state.field_74 == 0) {
            advance_palette_record_countdown_slot06_10((Slot06Pal *)((u8 *)obj + 0x30));
            advance_palette_record_countdown_slot06_10((Slot06Pal *)((u8 *)obj + 0x40));
            if (((Slot06Obj *)obj)->field_46 != 0) {
                ((Slot06Obj *)obj)->pos_x = 0x340;
                ((Slot06Obj *)obj)->pos_y = 0x80;
                *(u16 *)&((Slot06Obj *)obj)->field_46 = *(u16 *)&((Slot06Obj *)obj)->field_46 & 0xff00;
            }
            advance_palette_record_countdown_slot06_10((Slot06Pal *)((u8 *)obj + 0x50));
            if (((Slot06Obj *)obj)->field_56 != 0) {
                ((Slot06Obj *)obj)->pos_x = 0x1c0;
                ((Slot06Obj *)obj)->pos_y = 0x80;
                *(u16 *)&((Slot06Obj *)obj)->field_56 = *(u16 *)&((Slot06Obj *)obj)->field_56 & 0xff00;
            }
            advance_record_cursor_slot06_10((Slot06Cursor *)((u8 *)obj + 0x28));
            advance_record_cursor_slot06_10((Slot06Cursor *)((u8 *)obj + 0x6c));
            advance_record_cursor_slot06_10((Slot06Cursor *)((u8 *)obj + 0x84));
        }
    }
}

void release_object_record_slot06_10(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void advance_palette_record_countdown_slot06_10(Slot06Pal *pal) {
    SeqRec *rec;
    int v = pal->countdown - 1;
    pal->countdown = v;
    if ((s16)v == 0) {
        rec = pal->rec;
        rec++;
        if ((s16)pal->step_value < 0) {
            rec = (SeqRec *)rec->header;
        }
        set_palette_record_and_copy_data_slot06_10(pal, rec);
    }
}

void load_palette_record_by_index_slot06_10(Slot06Pal *pal) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    set_palette_record_and_copy_data_slot06_10(pal, (SeqRec *)table_8017c910[pal->seq_rec_index]);
}
