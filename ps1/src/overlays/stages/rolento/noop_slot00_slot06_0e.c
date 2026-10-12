/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void noop_slot00_slot06_0e(Object *obj);
void advance_cursor_record_countdown_slot06_0e(Slot06Cursor *cur);
void load_palette_record_by_index_slot06_0e(Slot06Pal *pal);
void set_palette_record_and_copy_data_slot06_0e(Slot06Pal *pal, SeqRec *rec);

void noop_slot00_slot06_0e(Object *obj) {
}

void release_object_record_slot06_0e(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void advance_cursor_record_countdown_slot06_0e(Slot06Cursor *cur) {
    SeqRec *rec;
    int v = cur->remaining_count - 1;
    cur->remaining_count = v;
    if ((s16)v == 0) {
        rec = cur->rec;
        rec++;
        if ((s16)cur->step_word < 0) {
            rec = (SeqRec *)rec->header;
        }
        set_palette_record_and_copy_data_slot06_0e((Slot06Pal *)cur, rec);
    }
}

void load_palette_record_by_index_slot06_0e(Slot06Pal *pal) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    set_palette_record_and_copy_data_slot06_0e(pal, (SeqRec *)table_8017c910[pal->seq_rec_index]);
}
