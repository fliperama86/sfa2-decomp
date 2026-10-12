/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void load_palette_record_by_index_slot06_0c(Slot06Pal *pal);
void advance_cursor_record_countdown_slot06_0c(Slot06Cursor *cur);
void set_palette_record_and_copy_data_slot06_0c(Slot06Pal *pal, SeqRec *rec);

void update_three_sequence_cursors_slot06_0c(Object *obj) {
    if (game_state.field_74 == 0) {
        advance_cursor_record_countdown_slot06_0c((Slot06Cursor *)((u8 *)obj + 0x5c));
        advance_cursor_record_countdown_slot06_0c((Slot06Cursor *)((u8 *)obj + 0x70));
        advance_cursor_record_countdown_slot06_0c((Slot06Cursor *)((u8 *)obj + 0x4c));
    }
    set_flag_and_push_if_mode_8(obj);
}

void release_object_record_slot06_0c(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void advance_cursor_record_countdown_slot06_0c(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->remaining_count - 1;
    cur->remaining_count = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->step_word < 0) {
            rec = (SeqRec *)rec->header;
        }
        set_palette_record_and_copy_data_slot06_0c((Slot06Pal *)cur, rec);
    }
}

void load_palette_record_by_index_slot06_0c(Slot06Pal *pal) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    set_palette_record_and_copy_data_slot06_0c(pal, (SeqRec *)table_8017c910[pal->seq_rec_index]);
}
